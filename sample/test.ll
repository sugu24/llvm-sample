; ModuleID = './sample/test.dc'

declare i32 @printnum(i32)

define i32 @test(i32 %j_arg) {
entry:
  %call_temp = call i32 @printnum(i32 %j_arg)
  ret i32 0
}

define i32 @main() {
entry:
  %call_temp = call i32 @test(i32 10)
  ret i32 0
}
