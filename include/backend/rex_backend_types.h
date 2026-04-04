#include "mlir/IR/Types.h"

namespace rex {

struct BackendType {

    public:
        mlir::Type i32;
        mlir::Type c8;
        mlir::Type f32;
        mlir::Type b1;
        mlir::Type ptr;
};

} // namespace rex
