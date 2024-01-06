declare i32 @getch( )
declare i32 @getint( )
declare void @putch( i32 )
declare void @putint( i32 )
declare void @putarray( i32, i32* )
declare void @_sysy_starttime( i32 )
declare void @_sysy_stoptime( i32 )
define void @main( ) {
bb2:
  %r121 = add i32 0, 0
  %r122 = add i32 0, 0
  %r123 = add i32 0, 0
  %r124 = add i32 10, 0
  %r125 = add i32 0, 0
  br label %bb_while_5_5_cond

bb_while_5_5_cond:
  %r126 = phi i32 [ %r125, %bb2 ], [ %r129, %bb_if_9_9_end ]
  %r127 = phi i32 [ %r123, %bb2 ], [ %r128, %bb_if_9_9_end ]
  %r104 = icmp slt i32 %r126, %r124
  br i1 %r104, label %bb_while_5_5_whilestms, label %bb_while_5_5_bre

bb_while_5_5_whilestms:
  call void @putint(i32 %r126)
  %r108 = sdiv i32 %r126, 2
  %r128 = add i32 %r108, 0
  %r111 = mul i32 %r128, 2
  %r112 = sub i32 %r126, %r111
  %r113 = icmp eq i32 %r112, 1
  br i1 %r113, label %bb_if_9_9_true, label %bb29_9checksecond

bb29_9checksecond:
  %r115 = icmp eq i32 %r126, 4
  br i1 %r115, label %bb_if_9_9_true, label %bb_if_9_9_false

bb_if_9_9_true:
  call void @putch(i32 33)
  br label %bb_if_9_9_end

bb_if_9_9_false:
  br label %bb_if_9_9_end

bb_if_9_9_end:
  call void @putch(i32 32)
  %r117 = add i32 %r126, 1
  %r129 = add i32 %r117, 0
  br label %bb_while_5_5_cond

bb_while_5_5_bre:
  ret void
}

