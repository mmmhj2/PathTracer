#ifndef SHADOW_RAY_TRACE_H_INCLUDED
#define SHADOW_RAY_TRACE_H_INCLUDED

#include "objects/objlist_base.h"
//#include "material/pdf/pdf_object.h"
#include "ray/light/light.h"

double heuristic_weight(int nf, double fPdf, int ng, double gPdf)
{
    double f, g;
    f = nf * fPdf, g = ng * gPdf;
    return (f * f) / (f * f + g * g);
//    return f / (f + g);
}

light_sample sample_lights(const hit_record & rec, const light_list * lights)
{
    size_t sz = lights->size();
    return (*lights)[tools::random_int(0, sz-1)]->sample_Li(rec);
}

color trace_shadow_ray(const ray & r_out,
                       const hit_record & rec,
                       const objlist_base & world,
                       const skybox_base & skybox,
                       const light_list * lights)
{
    color result(0., 0., 0.), scatter_color;
    double lightPdf = 0, scatteringPdf = 0;
    light_sample sample = sample_lights(rec, lights);
    // Acquire BSDF
    auto mat_ptr = rec.obj->get_material();
    std::shared_ptr <BSDF_base> bsdf_ptr;
    mat_ptr->evaluateScatter(r_out, rec, bsdf_ptr);

    if(sample.pdf > 0)
    {
        // Evaluate scattering BSDF
        scatter_color = bsdf_ptr->eval_raw(r_out, sample.shadow_ray);
        scatteringPdf = bsdf_ptr->pdf(r_out, sample.shadow_ray);
        lightPdf = sample.pdf;

        if(!tools::is_near_zero(scatter_color) && !sample.is_occluded)
        {
            color emissive;

            std::shared_ptr <BSDF_base> light_bsdf_ptr;
            sample.shadow_hitrec.obj->get_material()->evaluateEmissive(sample.shadow_ray, sample.shadow_hitrec, light_bsdf_ptr);
            emissive = light_bsdf_ptr->eval(sample.shadow_ray, sample.shadow_ray);
            emissive = elem_product(scatter_color, emissive);

            if(sample.light->is_delta_light())
                result += emissive / lightPdf;
            else
            {
                double weight = heuristic_weight(1, lightPdf, 1, scatteringPdf);
                result += emissive * weight / lightPdf;
            }
        }
    }

    if(!sample.light->is_delta_light())
    {
        // Sample BSDF
        ray sample_ray;
        bsdf_ptr->sample(r_out, sample_ray);
        scatter_color = bsdf_ptr->eval(r_out, sample_ray);
        scatteringPdf = bsdf_ptr->pdf(r_out, sample_ray);

        if(!tools::is_near_zero(scatter_color) && scatteringPdf > 0)
        {
            double weight = 1;
            if(!bsdf_ptr->is_delta_bsdf())
            {
                lightPdf = sample.light->pdf_Li(sample_ray);
                if(lightPdf <= 1e-9)
                    return result;
                weight = heuristic_weight(1, scatteringPdf, 1, lightPdf);
            }

            // Generate contribution of light source
            if(sample.light->is_samplable(sample_ray))
            {
                color light_color;
                sample.light->evaluate()
            }
        }
    }
    return result;
}

#endif // SHADOW_RAY_TRACE_H_INCLUDED
