; ModuleID = 'LLVMDialectModule'
source_filename = "LLVMDialectModule"

@str_const_0 = internal constant [11 x i8] c"The Blight\00"
@fmt_int = internal constant [3 x i8] c"%d\00"
@fmt_float = internal constant [3 x i8] c"%f\00"
@fmt_char = internal constant [3 x i8] c"%c\00"
@fmt_string = internal constant [3 x i8] c"%s\00"

declare i32 @printf(ptr, ...)

define i32 @main() {
  %1 = call i32 (ptr, ...) @printf(ptr @fmt_int, i32 234)
  %2 = call i32 (ptr, ...) @printf(ptr @fmt_char, i8 10)
  %3 = call i32 (ptr, ...) @printf(ptr @fmt_float, double 0x400BA40B80000000)
  %4 = call i32 (ptr, ...) @printf(ptr @fmt_char, i8 10)
  %5 = call i32 (ptr, ...) @printf(ptr @fmt_char, i8 10)
  %6 = call i32 (ptr, ...) @printf(ptr @fmt_char, i8 10)
  %7 = call i32 (ptr, ...) @printf(ptr @fmt_int, i32 1)
  %8 = call i32 (ptr, ...) @printf(ptr @fmt_char, i8 10)
  %9 = call i32 (ptr, ...) @printf(ptr @fmt_string, ptr @str_const_0)
  %10 = call i32 (ptr, ...) @printf(ptr @fmt_char, i8 10)
  ret i32 0
}
