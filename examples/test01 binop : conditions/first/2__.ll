declare i32 @getch( )
declare i32 @getint( )
declare void @putch( i32 )
declare void @putint( i32 )
declare void @putarray( i32, i32* )
declare void @_sysy_starttime( i32 )
declare void @_sysy_stoptime( i32 )
define void @main( ) {
bb2:
  %r100 = alloca i32
  %r101 = alloca i32
  %r106 = alloca i32
bb1:
  store i32 10, i32* %r100
  store i32 0, i32* %r101
  br label %bb_while_5_5_cond

bb_while_5_5_cond:
  %r102 = load i32, i32* %r101
  %r103 = load i32, i32* %r100
  %r104 = icmp slt i32 %r102, %r103
  br i1 %r104, label %bb_while_5_5_whilestms, label %bb_while_5_5_bre

bb_while_5_5_whilestms:
  %r105 = load i32, i32* %r101
  call void @putint(i32 %r105)
  %r107 = load i32, i32* %r101
  %r108 = sdiv i32 %r107, 2
  store i32 %r108, i32* %r106
  %r109 = load i32, i32* %r101
  %r110 = load i32, i32* %r106
  %r111 = mul i32 %r110, 2
  %r112 = sub i32 %r109, %r111
  %r113 = icmp eq i32 %r112, 1
  br i1 %r113, label %bb_if_9_9_true, label %bb29_9checksecond

bb29_9checksecond:
  %r114 = load i32, i32* %r101
  %r115 = icmp eq i32 %r114, 4
  br i1 %r115, label %bb_if_9_9_true, label %bb_if_9_9_false

bb_if_9_9_true:
  call void @putch(i32 33)
  br label %bb_if_9_9_end

bb_if_9_9_false:
  br label %bb_if_9_9_end

bb_if_9_9_end:
  call void @putch(i32 32)
  %r116 = load i32, i32* %r101
  %r117 = add i32 %r116, 1
  store i32 %r117, i32* %r101
  br label %bb_while_5_5_cond

bb_while_5_5_bre:
  ret void
}

