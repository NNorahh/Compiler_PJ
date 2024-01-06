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
  call void @putch(i32 32)
  %r106 = load i32, i32* %r101
  %r107 = add i32 %r106, 1
  store i32 %r107, i32* %r101
  br label %bb_while_5_5_cond

bb_while_5_5_bre:
  ret void
}

