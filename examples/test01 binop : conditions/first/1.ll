declare i32 @getch( )
declare i32 @getint( )
declare void @putch( i32 )
declare void @putint( i32 )
declare void @putarray( i32, i32* )
declare void @_sysy_starttime( i32 )
declare void @_sysy_stoptime( i32 )
define void @main( ) {
bb2:
  %r110 = add i32 0, 0
  %r111 = add i32 0, 0
  %r112 = add i32 10, 0
  %r113 = add i32 0, 0
  br label %bb_while_5_5_cond

bb_while_5_5_cond:
  %r114 = phi i32 [ %r113, %bb2 ], [ %r115, %bb_while_5_5_whilestms ]
  %r104 = icmp slt i32 %r114, %r112
  br i1 %r104, label %bb_while_5_5_whilestms, label %bb_while_5_5_bre

bb_while_5_5_whilestms:
  call void @putint(i32 %r114)
  call void @putch(i32 32)
  %r107 = add i32 %r114, 1
  %r115 = add i32 %r107, 0
  br label %bb_while_5_5_cond

bb_while_5_5_bre:
  ret void
}

