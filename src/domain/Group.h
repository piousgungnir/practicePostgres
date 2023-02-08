#pragma once


#include <memory>
#include <vector>
#include <list>

#include "Answer.h"


using Response = std::shared_ptr<const impl::Answer>;
using Group = std::vector<Response>;
using Groups = std::list<Group>;

