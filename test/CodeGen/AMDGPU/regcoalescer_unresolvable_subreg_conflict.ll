; RUN: llc -march=amdgcn < %s | FileCheck %s

; CHECK: main:

; Function Attrs: nounwind
define amdgpu_ps void @main() local_unnamed_addr #0 {
.entry:
  br i1 undef, label %7, label %0

; <label>:0:                                      ; preds = %.entry
  br i1 undef, label %5, label %1, !llvm.loop !1

; <label>:1:                                      ; preds = %0
  br i1 undef, label %._crit_edge3502, label %.lr.ph3501

.lr.ph3501:                                       ; preds = %1
  br label %2

; <label>:2:                                      ; preds = %4, %.lr.ph3501
  br i1 undef, label %3, label %4

; <label>:3:                                      ; preds = %2
  br label %4, !llvm.loop !2

; <label>:4:                                      ; preds = %3, %2
  br i1 undef, label %._crit_edge3502, label %2

._crit_edge3502:                                  ; preds = %4, %1
  br label %5, !llvm.loop !3

; <label>:5:                                      ; preds = %._crit_edge3502, %0
  br i1 undef, label %.lr.ph3482, label %.loopexit, !llvm.loop !4

.lr.ph3482:                                       ; preds = %5
  br label %6

; <label>:6:                                      ; preds = %6, %.lr.ph3482
  br i1 undef, label %6, label %..loopexit_crit_edge, !llvm.loop !4

; <label>:7:                                      ; preds = %.entry
  br label %.loopexit, !llvm.loop !5

..loopexit_crit_edge:                             ; preds = %6
  br label %.loopexit

.loopexit:                                        ; preds = %..loopexit_crit_edge, %7, %5
  br i1 undef, label %20, label %8, !llvm.loop !6

; <label>:8:                                      ; preds = %.loopexit
  br i1 undef, label %13, label %9, !llvm.loop !7

; <label>:9:                                      ; preds = %8
  br i1 undef, label %11, label %10, !llvm.loop !8

; <label>:10:                                     ; preds = %9
  br label %12, !llvm.loop !9

; <label>:11:                                     ; preds = %9
  br label %12, !llvm.loop !10

; <label>:12:                                     ; preds = %11, %10
  br label %.lr.ph3459, !llvm.loop !11

; <label>:13:                                     ; preds = %8
  br label %.lr.ph3459, !llvm.loop !12

.lr.ph3459:                                       ; preds = %13, %12
  br label %14

; <label>:14:                                     ; preds = %._crit_edge3438, %.lr.ph3459
  br i1 undef, label %.lr.ph3437, label %._crit_edge3438, !llvm.loop !13

.lr.ph3437:                                       ; preds = %14
  br label %15

; <label>:15:                                     ; preds = %19, %.lr.ph3437
  br i1 undef, label %19, label %16, !llvm.loop !14

; <label>:16:                                     ; preds = %15
  br i1 undef, label %18, label %17, !llvm.loop !15

; <label>:17:                                     ; preds = %16
  br label %18, !llvm.loop !16

; <label>:18:                                     ; preds = %17, %16
  br label %19, !llvm.loop !17

; <label>:19:                                     ; preds = %18, %15
  br i1 undef, label %15, label %._crit_edge3438, !llvm.loop !13

._crit_edge3438:                                  ; preds = %19, %14
  br i1 undef, label %14, label %._crit_edge3460, !llvm.loop !18

._crit_edge3460:                                  ; preds = %._crit_edge3438
  br label %20, !llvm.loop !19

; <label>:20:                                     ; preds = %._crit_edge3460, %.loopexit
  br i1 undef, label %47, label %.lr.ph3410, !llvm.loop !20

.lr.ph3410:                                       ; preds = %20
  br label %21

; <label>:21:                                     ; preds = %._crit_edge3384, %.lr.ph3410
  br i1 undef, label %._crit_edge3384, label %.lr.ph3383, !llvm.loop !21

.lr.ph3383:                                       ; preds = %21
  br label %22

; <label>:22:                                     ; preds = %33, %.lr.ph3383
  br i1 undef, label %24, label %23, !llvm.loop !22

; <label>:23:                                     ; preds = %22
  br label %33, !llvm.loop !23

; <label>:24:                                     ; preds = %22
  br i1 undef, label %26, label %25, !llvm.loop !24

; <label>:25:                                     ; preds = %24
  br label %33, !llvm.loop !25

; <label>:26:                                     ; preds = %24
  br i1 undef, label %28, label %27, !llvm.loop !26

; <label>:27:                                     ; preds = %26
  br label %29, !llvm.loop !27

; <label>:28:                                     ; preds = %26
  br label %29, !llvm.loop !28

; <label>:29:                                     ; preds = %28, %27
  br i1 undef, label %31, label %30, !llvm.loop !29

; <label>:30:                                     ; preds = %29
  br label %32, !llvm.loop !30

; <label>:31:                                     ; preds = %29
  br label %32, !llvm.loop !31

; <label>:32:                                     ; preds = %31, %30
  br label %33, !llvm.loop !32

; <label>:33:                                     ; preds = %32, %25, %23
  br i1 undef, label %._crit_edge3384, label %22, !llvm.loop !21

._crit_edge3384:                                  ; preds = %33, %21
  br i1 undef, label %21, label %._crit_edge3411, !llvm.loop !33

._crit_edge3411:                                  ; preds = %._crit_edge3384
  br label %34

; <label>:34:                                     ; preds = %._crit_edge3337, %._crit_edge3411
  br i1 undef, label %._crit_edge3337, label %.lr.ph3336, !llvm.loop !34

.lr.ph3336:                                       ; preds = %34
  br label %35

; <label>:35:                                     ; preds = %46, %.lr.ph3336
  br i1 undef, label %37, label %36, !llvm.loop !35

; <label>:36:                                     ; preds = %35
  br label %46, !llvm.loop !36

; <label>:37:                                     ; preds = %35
  br i1 undef, label %39, label %38, !llvm.loop !37

; <label>:38:                                     ; preds = %37
  br label %46, !llvm.loop !38

; <label>:39:                                     ; preds = %37
  br i1 undef, label %41, label %40, !llvm.loop !39

; <label>:40:                                     ; preds = %39
  br label %42, !llvm.loop !40

; <label>:41:                                     ; preds = %39
  br label %42, !llvm.loop !41

; <label>:42:                                     ; preds = %41, %40
  br i1 undef, label %44, label %43, !llvm.loop !42

; <label>:43:                                     ; preds = %42
  br label %45, !llvm.loop !43

; <label>:44:                                     ; preds = %42
  br label %45, !llvm.loop !44

; <label>:45:                                     ; preds = %44, %43
  br label %46, !llvm.loop !45

; <label>:46:                                     ; preds = %45, %38, %36
  br i1 undef, label %._crit_edge3337, label %35, !llvm.loop !34

._crit_edge3337:                                  ; preds = %46, %34
  br i1 undef, label %34, label %._crit_edge3364, !llvm.loop !46

._crit_edge3364:                                  ; preds = %._crit_edge3337
  br label %47, !llvm.loop !47

; <label>:47:                                     ; preds = %._crit_edge3364, %20
  br i1 undef, label %85, label %.lr.ph3311, !llvm.loop !48

.lr.ph3311:                                       ; preds = %47
  br label %48

; <label>:48:                                     ; preds = %._crit_edge3284, %.lr.ph3311
  br i1 undef, label %._crit_edge3284, label %.lr.ph3283, !llvm.loop !49

.lr.ph3283:                                       ; preds = %48
  br label %49

; <label>:49:                                     ; preds = %79, %.lr.ph3283
  %__llpc_global_proxy_r10.133275 = phi <4 x i32> [ undef, %.lr.ph3283 ], [ %__llpc_global_proxy_r10.14, %79 ]
  %__llpc_global_proxy_r9.133274 = phi <4 x i32> [ undef, %.lr.ph3283 ], [ %__llpc_global_proxy_r9.14, %79 ]
  %50 = call i32 @llvm.cttz.i32(i32 undef, i1 true) #3, !range !50
  %51 = or i32 %50, 0
  %52 = mul i32 %51, 48
  %__llpc_global_proxy_r9.0.vec.insert1483 = insertelement <4 x i32> %__llpc_global_proxy_r9.133274, i32 undef, i32 0
  %53 = add i32 %52, 32
  %54 = call i32 @llvm.amdgcn.s.buffer.load.i32(<4 x i32> undef, i32 %53, i1 false) #3
  %55 = or i32 0, %54
  %56 = and i32 %55, 1
  %sext3225 = add nsw i32 %56, -1
  %57 = call i32 @llvm.amdgcn.s.buffer.load.i32(<4 x i32> undef, i32 undef, i1 false) #3
  %58 = bitcast i32 %57 to float
  %59 = fcmp oge float 0.000000e+00, %58
  %.03116 = sext i1 %59 to i32
  %__llpc_global_proxy_r10.12.vec.insert1883 = insertelement <4 x i32> %__llpc_global_proxy_r10.133275, i32 %.03116, i32 3
  %60 = or i32 %sext3225, %.03116
  %__llpc_global_proxy_r10.8.vec.insert1826 = insertelement <4 x i32> %__llpc_global_proxy_r10.12.vec.insert1883, i32 %60, i32 2
  %61 = icmp eq i32 %60, 0
  br i1 %61, label %63, label %62, !llvm.loop !51

; <label>:62:                                     ; preds = %49
  br label %79, !llvm.loop !52

; <label>:63:                                     ; preds = %49
  %__llpc_global_proxy_r10.8.vec.insert1836 = insertelement <4 x i32> %__llpc_global_proxy_r10.8.vec.insert1826, i32 undef, i32 2
  %__llpc_global_proxy_r10.12.vec.insert1923 = insertelement <4 x i32> %__llpc_global_proxy_r10.8.vec.insert1836, i32 undef, i32 3
  %64 = shufflevector <4 x i32> %__llpc_global_proxy_r10.133275, <4 x i32> undef, <2 x i32> <i32 0, i32 1>
  %65 = bitcast <2 x i32> %64 to <2 x float>
  %66 = fmul <2 x float> %65, undef
  %67 = fsub <2 x float> zeroinitializer, %66
  %68 = fmul <2 x float> %67, undef
  %69 = fadd <2 x float> undef, %68
  %x0.i267 = extractelement <2 x float> %69, i32 0
  %70 = call float @llvm.maxnum.f32(float %x0.i267, float 0.000000e+00) #3
  %71 = call float @llvm.log2.f32(float %70) #3
  %72 = fmul float undef, %71
  %73 = call float @llvm.exp2.f32(float %72) #3
  %74 = fmul float %73, undef
  %75 = bitcast float %74 to i32
  %__llpc_global_proxy_r9.0.vec.insert1639 = insertelement <4 x i32> %__llpc_global_proxy_r9.0.vec.insert1483, i32 %75, i32 0
  %76 = call float @llvm.exp2.f32(float undef) #3
  %77 = fmul float %76, undef
  %78 = bitcast float %77 to i32
  %__llpc_global_proxy_r8.12.vec.insert1319 = insertelement <4 x i32> undef, i32 %78, i32 3
  br label %79, !llvm.loop !53

; <label>:79:                                     ; preds = %63, %62
  %__llpc_global_proxy_r8.19 = phi <4 x i32> [ undef, %62 ], [ %__llpc_global_proxy_r8.12.vec.insert1319, %63 ]
  %__llpc_global_proxy_r9.14 = phi <4 x i32> [ %__llpc_global_proxy_r9.0.vec.insert1483, %62 ], [ %__llpc_global_proxy_r9.0.vec.insert1639, %63 ]
  %__llpc_global_proxy_r10.14 = phi <4 x i32> [ %__llpc_global_proxy_r10.8.vec.insert1826, %62 ], [ %__llpc_global_proxy_r10.12.vec.insert1923, %63 ]
  %__llpc_global_proxy_r8.8.vec.extract1272 = extractelement <4 x i32> %__llpc_global_proxy_r8.19, i32 2
  br i1 false, label %._crit_edge3284, label %49, !llvm.loop !49

._crit_edge3284:                                  ; preds = %79, %48
  br i1 undef, label %48, label %._crit_edge3312, !llvm.loop !54

._crit_edge3312:                                  ; preds = %._crit_edge3284
  br label %80

; <label>:80:                                     ; preds = %._crit_edge, %._crit_edge3312
  br i1 undef, label %._crit_edge, label %.lr.ph, !llvm.loop !55

.lr.ph:                                           ; preds = %80
  br label %81

; <label>:81:                                     ; preds = %84, %.lr.ph
  br i1 undef, label %83, label %82, !llvm.loop !56

; <label>:82:                                     ; preds = %81
  br label %84, !llvm.loop !57

; <label>:83:                                     ; preds = %81
  br label %84, !llvm.loop !58

; <label>:84:                                     ; preds = %83, %82
  br i1 undef, label %._crit_edge, label %81, !llvm.loop !55

._crit_edge:                                      ; preds = %84, %80
  br i1 undef, label %80, label %._crit_edge3268, !llvm.loop !59

._crit_edge3268:                                  ; preds = %._crit_edge
  br label %85, !llvm.loop !60

; <label>:85:                                     ; preds = %._crit_edge3268, %47
  br i1 undef, label %87, label %86, !llvm.loop !61

; <label>:86:                                     ; preds = %85
  br label %87, !llvm.loop !62

; <label>:87:                                     ; preds = %86, %85
  br i1 undef, label %"PixelEpilog(.exit", label %88, !llvm.loop !63

; <label>:88:                                     ; preds = %87
  br label %"PixelEpilog(.exit"

"PixelEpilog(.exit":                              ; preds = %88, %87
  ret void
}

; Function Attrs: nounwind readnone speculatable
declare float @llvm.maxnum.f32(float, float) #1

; Function Attrs: nounwind readnone speculatable
declare float @llvm.exp2.f32(float) #1

; Function Attrs: nounwind readnone speculatable
declare float @llvm.log2.f32(float) #1

; Function Attrs: nounwind readnone speculatable
declare i32 @llvm.cttz.i32(i32, i1) #1

; Function Attrs: nounwind readnone
declare i32 @llvm.amdgcn.s.buffer.load.i32(<4 x i32>, i32, i1) #2

attributes #0 = { nounwind "InitialPSInputAddr"="3841" }
attributes #1 = { nounwind readnone speculatable }
attributes #2 = { nounwind readnone }
attributes #3 = { nounwind }

!spirv.Generator = !{!0}

!0 = !{i16 8, i16 1}
!1 = distinct !{!1}
!2 = distinct !{!2}
!3 = distinct !{!3}
!4 = distinct !{!4}
!5 = distinct !{!5}
!6 = distinct !{!6}
!7 = distinct !{!7}
!8 = distinct !{!8}
!9 = distinct !{!9}
!10 = distinct !{!10}
!11 = distinct !{!11}
!12 = distinct !{!12}
!13 = distinct !{!13}
!14 = distinct !{!14}
!15 = distinct !{!15}
!16 = distinct !{!16}
!17 = distinct !{!17}
!18 = distinct !{!18}
!19 = distinct !{!19}
!20 = distinct !{!20}
!21 = distinct !{!21}
!22 = distinct !{!22}
!23 = distinct !{!23}
!24 = distinct !{!24}
!25 = distinct !{!25}
!26 = distinct !{!26}
!27 = distinct !{!27}
!28 = distinct !{!28}
!29 = distinct !{!29}
!30 = distinct !{!30}
!31 = distinct !{!31}
!32 = distinct !{!32}
!33 = distinct !{!33}
!34 = distinct !{!34}
!35 = distinct !{!35}
!36 = distinct !{!36}
!37 = distinct !{!37}
!38 = distinct !{!38}
!39 = distinct !{!39}
!40 = distinct !{!40}
!41 = distinct !{!41}
!42 = distinct !{!42}
!43 = distinct !{!43}
!44 = distinct !{!44}
!45 = distinct !{!45}
!46 = distinct !{!46}
!47 = distinct !{!47}
!48 = distinct !{!48}
!49 = distinct !{!49}
!50 = !{i32 0, i32 33}
!51 = distinct !{!51}
!52 = distinct !{!52}
!53 = distinct !{!53}
!54 = distinct !{!54}
!55 = distinct !{!55}
!56 = distinct !{!56}
!57 = distinct !{!57}
!58 = distinct !{!58}
!59 = distinct !{!59}
!60 = distinct !{!60}
!61 = distinct !{!61}
!62 = distinct !{!62}
!63 = distinct !{!63}
