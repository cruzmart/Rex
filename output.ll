; ModuleID = 'LLVMDialectModule'
source_filename = "LLVMDialectModule"

@fmt_int = internal constant [4 x i8] c"%d\0A\00"
@fmt_float = internal constant [4 x i8] c"%f\0A\00"
@fmt_char = internal constant [4 x i8] c"%c\0A\00"
@fmt_string = internal constant [4 x i8] c"%s\0A\00"

declare i32 @printf(ptr, ...)

define i32 @main() {
  %1 = call i32 (ptr, ...) @printf(ptr @fmt_int, i32 42)
  %2 = call i32 (ptr, ...) @printf(ptr @fmt_float, double 0x40091EB860000000)
  %3 = call i32 (ptr, ...) @printf(ptr @fmt_char, i8 65)
  %4 = call i32 (ptr, ...) @printf(ptr @fmt_int, i32 1)
  %5 = call i32 (ptr, ...) @printf(ptr @fmt_int, i32 42)
  ret i32 0
}
