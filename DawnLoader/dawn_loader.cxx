#include "dawn/dawn_proc.h"
#include "dawn/dawn_proc_table.h"
#include "dawn/native/DawnNative.h"

#include "dawn_loader_export.h"

namespace {
struct DawnInitializer {
  DawnInitializer() {
    auto procs = dawn::native::GetProcs();
    dawnProcSetProcs(&procs);
  }
  ~DawnInitializer() { dawnProcSetProcs(nullptr); }
};

std::unique_ptr<DawnInitializer> Initializer;
} // namespace

extern "C" {
DAWN_LOADER_EXPORT void initDawn() { ::Initializer.reset(new DawnInitializer()); }

DAWN_LOADER_EXPORT void finalizeDawn() { ::Initializer.reset(nullptr); }
}
