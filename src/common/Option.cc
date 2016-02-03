#include "common/Option.hpp"

Type* Option::get() {
   if (!empty) {
       return item;
   } else {
       throw 20;
   }
}
