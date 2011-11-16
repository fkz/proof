#include "application.h"


template< class T >
T* Element::cast()
{
  T *result = dynamic_cast< T * > (this);
  if (!result) {
    Application *appl = dynamic_cast< Application * > (this);
    if (!appl)
      return 0;
    Element* ele = appl->apply();
    result = ele->cast< T > ();
    ele->remove ();
  }
  return dynamic_cast< T * > (result->copy());
}
