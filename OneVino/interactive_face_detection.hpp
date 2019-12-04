#ifndef INTERACTIVE_FACE_DETECTION_H
#define INTERACTIVE_FACE_DETECTION_H


// Copyright (C) 2018-2019 Intel Corporation
// SPDX-License-Identifier: Apache-2.0
//

///////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <string>
#include <vector>
#include <gflags/gflags.h>
#include <iostream>

#define FLAGS_h false
#define FLAGS_i ""
#define FLAGS_o ""

#define FLAGS_m  "/root/OneVino/models/FP16/face-detection-adas-0001.xml"

#define FLAGS_m_ag "/root/OneVino/models/FP16/age-gender-recognition-retail-0013.xml"

#define FLAGS_m_hp "/root/OneVino/models/FP16/head-pose-estimation-adas-0001.xml"

#define FLAGS_m_em "/root/OneVino/models/FP16/emotions-recognition-retail-0003.xml"

#define FLAGS_m_lm  "/root/OneVino/models/FP16/landmarks-regression-retail-0009.xml"

#define FLAGS_m_reid "/root/OneVino/models/FP32/face-reidentification-retail-0095.xml"

#define FLAGS_d_reid  "CPU"

#define FLAGS_fg  "/root/OneVino/Identify/faces_gallery.json"

#define FLAGS_d  "CPU"

#define FLAGS_d_ag  "CPU"

#define FLAGS_d_hp  "CPU"

#define FLAGS_d_em  "CPU"

#define FLAGS_d_lm  "CPU"

#define FLAGS_d_va  "CPU"

#define FLAGS_d_lpr "CPU"

#define FLAGS_n_ag  16

#define FLAGS_dyn_ag  false

#define FLAGS_n_hp  16

#define FLAGS_dyn_hp  false

#define FLAGS_n_em  16

#define FLAGS_dyn_em  false

#define FLAGS_n_lm  16

#define FLAGS_dyn_lm false

#define FLAGS_pc  false

#define FLAGS_c  ""

#define FLAGS_l  ""

#define FLAGS_r  false

#define FLAGS_t  0.5

#define FLAGS_bb_enlarge_coef  1.2

#define FLAGS_no_wait  false

#define FLAGS_no_show false

#define FLAGS_async  false

#define FLAGS_dx_coef  1


#define FLAGS_dy_coef  1

#define FLAGS_fps -1

#define FLAGS_loop_video  true


#define FLAGS_no_smooth false

#define FLAGS_no_show_emotion_bar false

#define FLAGS_nc 0

#define FLAGS_ni 0

#define FLAGS_m_va ""

#define FLAGS_auto_resize false

#define FLAGS_mp "/root/OneVino/models/FP16/vehicle-license-plate-detection-barrier-0106.xml"

#define FLAGS_m_lpr "/root/OneVino/models/FP16/license-plate-recognition-barrier-0001.xml"

#define FLAGS_tag false

#define FLAGS_nireq 0

#define FLAGS_n_wt 1

#define FLAGS_n_iqs 3

/**
* \brief This function shows a help message
*/

#endif
