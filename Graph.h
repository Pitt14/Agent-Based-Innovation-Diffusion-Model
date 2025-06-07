#pragma once

#if NETWORK_MODEL == PREDEFINED_TOPOLOGY
#include "SocialNetwork.h"

#elif NETWORK_MODEL == COMPLETE_GRAPH
#include "CompleteGraph.h"

#elif NETWORK_MODEL == THEORETICAL_GRAPH
#include "TheoreticalGraph.h"

#endif