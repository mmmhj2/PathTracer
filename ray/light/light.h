#ifndef LIGHT_H_INCLUDED
#define LIGHT_H_INCLUDED

#include "light_base.h"
#include "light_shape.h"

typedef std::shared_ptr <light_base> light_ptr;
typedef std::vector <light_ptr> light_list;

#endif // LIGHT_H_INCLUDED
