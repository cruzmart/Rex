; ModuleID = 'LLVMDialectModule'
source_filename = "LLVMDialectModule"

@fmt_int = internal constant [3 x i8] c"%d\00"
@fmt_float = internal constant [3 x i8] c"%f\00"
@fmt_char = internal constant [3 x i8] c"%c\00"
@fmt_string = internal constant [3 x i8] c"%s\00"

declare i32 @printf(ptr, ...)

define i32 @main() {
  ret i32 0
}
