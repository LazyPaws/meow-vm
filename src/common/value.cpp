#include "common/value.h"

using namespace meow::common;

Int Value::toInt() {
    visit(
        [](Null)
    );
}