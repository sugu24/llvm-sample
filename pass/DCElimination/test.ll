; ModuleID = 'test.ll'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i32 @test(i32 %i, i32 %j) nounwind uwtable {
entry:
  %cmp = icmp sgt i32 %j, 0
  br i1 %cmp, label %if.then, label %if.else

if.then:                                          ; preds = %entry
  %add = add nsw i32 %i, %j
  br label %if.end

if.else:                                          ; preds = %entry
  %sub = sub nsw i32 %i, %j
  br label %if.end

if.end:                                           ; preds = %if.else, %if.then
  %k.0 = phi i32 [ %add, %if.then ], [ %sub, %if.else ]
  %mul = mul nsw i32 %k.0, 10
  ret i32 0
}
