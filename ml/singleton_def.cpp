#include "sysml.hpp"
#include "__mem.hpp"

lang::__p::__mem *lang::__p::__mem::__instance = new lang::__p::__mem{};

lang::tsafe_ptr<ml::display> ml::display::interface{new ml::display{}};

ptr<ml::cfg> ml::cfg::global{new ml::cfg{}};

ptr<ml::flags> ml::flags::global{new ml::flags{}};
