#pragma once

#include <memory>

namespace QtNodes{ class DataModelRegistry; };

std::shared_ptr<QtNodes::DataModelRegistry> registerDataModels();
