; RUN: llc -march=amdgcn -mattr=+vgpr-spilling < %s | FileCheck %s

; CHECK: main:

; ModuleID = '<stdin>'
source_filename = "bugpoint-output-e62c1a6.bc"
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v16:16:16-v24:32:32-v32:32:32-v48:64:64-v64:64:64-v96:128:128-v128:128:128-v192:256:256-v256:256:256-v512:512:512-v1024:1024:1024"
target triple = "spir64-unknown-unknown"

; Function Attrs: nounwind
define amdgpu_cs void @main() local_unnamed_addr #0 {
.entry:
  br i1 undef, label %._crit_edge, label %.lr.ph

.lr.ph:                                           ; preds = %.entry
  br label %0

; <label>:0:                                      ; preds = %24, %.lr.ph
  br label %1

; <label>:1:                                      ; preds = %1, %0
  %__llpc_global_proxy_r9.11793 = phi <4 x i32> [ undef, %0 ], [ %2, %1 ]
  %2 = shufflevector <4 x i32> %__llpc_global_proxy_r9.11793, <4 x i32> undef, <4 x i32> <i32 4, i32 5, i32 2, i32 6>
  br i1 undef, label %3, label %1, !llvm.loop !1

; <label>:3:                                      ; preds = %1
  %4 = shufflevector <4 x i32> undef, <4 x i32> %2, <4 x i32> <i32 0, i32 1, i32 6, i32 undef>
  %__llpc_global_proxy_r9.0.vec.insert = shufflevector <4 x i32> %4, <4 x i32> undef, <4 x i32> <i32 0, i32 1, i32 2, i32 6>
  br label %5

; <label>:5:                                      ; preds = %5, %3
  %__llpc_global_proxy_r5.12.vec.insert9681805 = phi <4 x i32> [ undef, %3 ], [ %__llpc_global_proxy_r5.12.vec.insert968, %5 ]
  %__llpc_global_proxy_r9.21801 = phi <4 x i32> [ %__llpc_global_proxy_r9.0.vec.insert, %3 ], [ undef, %5 ]
  %__llpc_global_proxy_r7.31800 = phi <4 x i32> [ undef, %3 ], [ %22, %5 ]
  %6 = shufflevector <4 x i32> %__llpc_global_proxy_r9.21801, <4 x i32> undef, <2 x i32> zeroinitializer
  %7 = add <2 x i32> %6, <i32 1, i32 6>
  %8 = load float, float* undef, align 16
  %9 = fmul float undef, %8
  %10 = fadd float undef, %9
  %11 = fadd float %10, undef
  %12 = bitcast float %11 to i32
  %__llpc_global_proxy_r5.12.vec.insert996 = insertelement <4 x i32> %__llpc_global_proxy_r5.12.vec.insert9681805, i32 %12, i32 3
  %13 = insertelement <3 x i32> undef, i32 %12, i32 0
  %14 = shufflevector <3 x i32> %13, <3 x i32> undef, <3 x i32> zeroinitializer
  %15 = bitcast <3 x i32> %14 to <3 x float>
  %16 = fmul <3 x float> undef, %15
  %17 = shufflevector <4 x i32> %__llpc_global_proxy_r7.31800, <4 x i32> undef, <3 x i32> <i32 0, i32 1, i32 3>
  %18 = bitcast <3 x i32> %17 to <3 x float>
  %19 = fadd <3 x float> %16, %18
  %20 = bitcast <3 x float> %19 to <3 x i32>
  %21 = shufflevector <3 x i32> %20, <3 x i32> undef, <4 x i32> <i32 0, i32 1, i32 2, i32 undef>
  %22 = shufflevector <4 x i32> %__llpc_global_proxy_r7.31800, <4 x i32> %21, <4 x i32> <i32 4, i32 5, i32 2, i32 6>
  %__llpc_global_proxy_r9.0.vec.extract1342 = extractelement <2 x i32> %7, i32 0
  %23 = icmp sgt i32 %__llpc_global_proxy_r9.0.vec.extract1342, 5
  %__llpc_global_proxy_r5.12.vec.insert968 = insertelement <4 x i32> %__llpc_global_proxy_r5.12.vec.insert996, i32 undef, i32 3
  br i1 %23, label %24, label %5, !llvm.loop !3

; <label>:24:                                     ; preds = %5
  %__llpc_global_proxy_r5.4.vec.extract = extractelement <4 x i32> %__llpc_global_proxy_r5.12.vec.insert9681805, i32 1
  %25 = icmp ult i32 0, %__llpc_global_proxy_r5.4.vec.extract
  br i1 %25, label %0, label %._crit_edge, !llvm.loop !4

._crit_edge:                                      ; preds = %24, %.entry
  ret void
}

attributes #0 = { nounwind }

!spirv.Generator = !{!0}

!0 = !{i16 8, i16 1}
!1 = distinct !{!1, !2}
!2 = !{!"llvm.loop.unroll.count", i32 128}
!3 = distinct !{!3, !2}
!4 = distinct !{!4, !2}
