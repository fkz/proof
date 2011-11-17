#include "element.h"

template< class T >
T* Element::cast()
{
  T *result = dynamic_cast< T * > (this);
  if (result)
    return dynamic_cast< T * > (result->copy());
  else {
    Element *ele = apply();
    if (!ele)
      return 0;
    else {
      result = ele->cast< T > ();
      ele->remove();
      return result;
    }
  }
}
