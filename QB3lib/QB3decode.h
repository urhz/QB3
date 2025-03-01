/*
Content: QB3 decoding

Copyright 2020-2023 Esri
Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at
http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

Contributors:  Lucian Plesea
*/

#pragma once
#include "QB3common.h"

namespace QB3 {
// Decoding tables, twice as large as the encoding ones
// 2k for 0-7
static const uint16_t drg0[] = { 0x1000, 0x1001, 0x1000, 0x1001 };
static const uint16_t drg1[] = { 0x1000, 0x2001, 0x1000, 0x3002, 0x1000, 0x2001, 0x1000, 0x3003 };
static const uint16_t drg2[] = { 0x2000, 0x3002, 0x2001, 0x4004, 0x2000, 0x3003, 0x2001, 0x4005, 0x2000, 0x3002, 0x2001, 0x4006,
0x2000, 0x3003, 0x2001, 0x4007 };
static const uint16_t drg3[] = { 0x3000, 0x4004, 0x3001, 0x5008, 0x3002, 0x4005, 0x3003, 0x5009, 0x3000, 0x4006, 0x3001, 0x500a,
0x3002, 0x4007, 0x3003, 0x500b, 0x3000, 0x4004, 0x3001, 0x500c, 0x3002, 0x4005, 0x3003, 0x500d, 0x3000, 0x4006, 0x3001, 0x500e,
0x3002, 0x4007, 0x3003, 0x500f };
static const uint16_t drg4[] = { 0x4000, 0x5008, 0x4001, 0x6010, 0x4002, 0x5009, 0x4003, 0x6011, 0x4004, 0x500a, 0x4005, 0x6012,
0x4006, 0x500b, 0x4007, 0x6013, 0x4000, 0x500c, 0x4001, 0x6014, 0x4002, 0x500d, 0x4003, 0x6015, 0x4004, 0x500e, 0x4005, 0x6016,
0x4006, 0x500f, 0x4007, 0x6017, 0x4000, 0x5008, 0x4001, 0x6018, 0x4002, 0x5009, 0x4003, 0x6019, 0x4004, 0x500a, 0x4005, 0x601a,
0x4006, 0x500b, 0x4007, 0x601b, 0x4000, 0x500c, 0x4001, 0x601c, 0x4002, 0x500d, 0x4003, 0x601d, 0x4004, 0x500e, 0x4005, 0x601e,
0x4006, 0x500f, 0x4007, 0x601f };
static const uint16_t drg5[] = { 0x5000, 0x6010, 0x5001, 0x7020, 0x5002, 0x6011, 0x5003, 0x7021, 0x5004, 0x6012, 0x5005, 0x7022,
0x5006, 0x6013, 0x5007, 0x7023, 0x5008, 0x6014, 0x5009, 0x7024, 0x500a, 0x6015, 0x500b, 0x7025, 0x500c, 0x6016, 0x500d, 0x7026,
0x500e, 0x6017, 0x500f, 0x7027, 0x5000, 0x6018, 0x5001, 0x7028, 0x5002, 0x6019, 0x5003, 0x7029, 0x5004, 0x601a, 0x5005, 0x702a,
0x5006, 0x601b, 0x5007, 0x702b, 0x5008, 0x601c, 0x5009, 0x702c, 0x500a, 0x601d, 0x500b, 0x702d, 0x500c, 0x601e, 0x500d, 0x702e,
0x500e, 0x601f, 0x500f, 0x702f, 0x5000, 0x6010, 0x5001, 0x7030, 0x5002, 0x6011, 0x5003, 0x7031, 0x5004, 0x6012, 0x5005, 0x7032,
0x5006, 0x6013, 0x5007, 0x7033, 0x5008, 0x6014, 0x5009, 0x7034, 0x500a, 0x6015, 0x500b, 0x7035, 0x500c, 0x6016, 0x500d, 0x7036,
0x500e, 0x6017, 0x500f, 0x7037, 0x5000, 0x6018, 0x5001, 0x7038, 0x5002, 0x6019, 0x5003, 0x7039, 0x5004, 0x601a, 0x5005, 0x703a,
0x5006, 0x601b, 0x5007, 0x703b, 0x5008, 0x601c, 0x5009, 0x703c, 0x500a, 0x601d, 0x500b, 0x703d, 0x500c, 0x601e, 0x500d, 0x703e,
0x500e, 0x601f, 0x500f, 0x703f };
static const uint16_t drg6[] = { 0x6000, 0x7020, 0x6001, 0x8040, 0x6002, 0x7021, 0x6003, 0x8041, 0x6004, 0x7022, 0x6005, 0x8042,
0x6006, 0x7023, 0x6007, 0x8043, 0x6008, 0x7024, 0x6009, 0x8044, 0x600a, 0x7025, 0x600b, 0x8045, 0x600c, 0x7026, 0x600d, 0x8046,
0x600e, 0x7027, 0x600f, 0x8047, 0x6010, 0x7028, 0x6011, 0x8048, 0x6012, 0x7029, 0x6013, 0x8049, 0x6014, 0x702a, 0x6015, 0x804a,
0x6016, 0x702b, 0x6017, 0x804b, 0x6018, 0x702c, 0x6019, 0x804c, 0x601a, 0x702d, 0x601b, 0x804d, 0x601c, 0x702e, 0x601d, 0x804e,
0x601e, 0x702f, 0x601f, 0x804f, 0x6000, 0x7030, 0x6001, 0x8050, 0x6002, 0x7031, 0x6003, 0x8051, 0x6004, 0x7032, 0x6005, 0x8052,
0x6006, 0x7033, 0x6007, 0x8053, 0x6008, 0x7034, 0x6009, 0x8054, 0x600a, 0x7035, 0x600b, 0x8055, 0x600c, 0x7036, 0x600d, 0x8056,
0x600e, 0x7037, 0x600f, 0x8057, 0x6010, 0x7038, 0x6011, 0x8058, 0x6012, 0x7039, 0x6013, 0x8059, 0x6014, 0x703a, 0x6015, 0x805a,
0x6016, 0x703b, 0x6017, 0x805b, 0x6018, 0x703c, 0x6019, 0x805c, 0x601a, 0x703d, 0x601b, 0x805d, 0x601c, 0x703e, 0x601d, 0x805e,
0x601e, 0x703f, 0x601f, 0x805f, 0x6000, 0x7020, 0x6001, 0x8060, 0x6002, 0x7021, 0x6003, 0x8061, 0x6004, 0x7022, 0x6005, 0x8062,
0x6006, 0x7023, 0x6007, 0x8063, 0x6008, 0x7024, 0x6009, 0x8064, 0x600a, 0x7025, 0x600b, 0x8065, 0x600c, 0x7026, 0x600d, 0x8066,
0x600e, 0x7027, 0x600f, 0x8067, 0x6010, 0x7028, 0x6011, 0x8068, 0x6012, 0x7029, 0x6013, 0x8069, 0x6014, 0x702a, 0x6015, 0x806a,
0x6016, 0x702b, 0x6017, 0x806b, 0x6018, 0x702c, 0x6019, 0x806c, 0x601a, 0x702d, 0x601b, 0x806d, 0x601c, 0x702e, 0x601d, 0x806e,
0x601e, 0x702f, 0x601f, 0x806f, 0x6000, 0x7030, 0x6001, 0x8070, 0x6002, 0x7031, 0x6003, 0x8071, 0x6004, 0x7032, 0x6005, 0x8072,
0x6006, 0x7033, 0x6007, 0x8073, 0x6008, 0x7034, 0x6009, 0x8074, 0x600a, 0x7035, 0x600b, 0x8075, 0x600c, 0x7036, 0x600d, 0x8076,
0x600e, 0x7037, 0x600f, 0x8077, 0x6010, 0x7038, 0x6011, 0x8078, 0x6012, 0x7039, 0x6013, 0x8079, 0x6014, 0x703a, 0x6015, 0x807a,
0x6016, 0x703b, 0x6017, 0x807b, 0x6018, 0x703c, 0x6019, 0x807c, 0x601a, 0x703d, 0x601b, 0x807d, 0x601c, 0x703e, 0x601d, 0x807e,
0x601e, 0x703f, 0x601f, 0x807f };
static const uint16_t drg7[] = { 0x7000, 0x8040, 0x7001, 0x9080, 0x7002, 0x8041, 0x7003, 0x9081, 0x7004, 0x8042, 0x7005, 0x9082,
0x7006, 0x8043, 0x7007, 0x9083, 0x7008, 0x8044, 0x7009, 0x9084, 0x700a, 0x8045, 0x700b, 0x9085, 0x700c, 0x8046, 0x700d, 0x9086,
0x700e, 0x8047, 0x700f, 0x9087, 0x7010, 0x8048, 0x7011, 0x9088, 0x7012, 0x8049, 0x7013, 0x9089, 0x7014, 0x804a, 0x7015, 0x908a,
0x7016, 0x804b, 0x7017, 0x908b, 0x7018, 0x804c, 0x7019, 0x908c, 0x701a, 0x804d, 0x701b, 0x908d, 0x701c, 0x804e, 0x701d, 0x908e,
0x701e, 0x804f, 0x701f, 0x908f, 0x7020, 0x8050, 0x7021, 0x9090, 0x7022, 0x8051, 0x7023, 0x9091, 0x7024, 0x8052, 0x7025, 0x9092,
0x7026, 0x8053, 0x7027, 0x9093, 0x7028, 0x8054, 0x7029, 0x9094, 0x702a, 0x8055, 0x702b, 0x9095, 0x702c, 0x8056, 0x702d, 0x9096,
0x702e, 0x8057, 0x702f, 0x9097, 0x7030, 0x8058, 0x7031, 0x9098, 0x7032, 0x8059, 0x7033, 0x9099, 0x7034, 0x805a, 0x7035, 0x909a,
0x7036, 0x805b, 0x7037, 0x909b, 0x7038, 0x805c, 0x7039, 0x909c, 0x703a, 0x805d, 0x703b, 0x909d, 0x703c, 0x805e, 0x703d, 0x909e,
0x703e, 0x805f, 0x703f, 0x909f, 0x7000, 0x8060, 0x7001, 0x90a0, 0x7002, 0x8061, 0x7003, 0x90a1, 0x7004, 0x8062, 0x7005, 0x90a2,
0x7006, 0x8063, 0x7007, 0x90a3, 0x7008, 0x8064, 0x7009, 0x90a4, 0x700a, 0x8065, 0x700b, 0x90a5, 0x700c, 0x8066, 0x700d, 0x90a6,
0x700e, 0x8067, 0x700f, 0x90a7, 0x7010, 0x8068, 0x7011, 0x90a8, 0x7012, 0x8069, 0x7013, 0x90a9, 0x7014, 0x806a, 0x7015, 0x90aa,
0x7016, 0x806b, 0x7017, 0x90ab, 0x7018, 0x806c, 0x7019, 0x90ac, 0x701a, 0x806d, 0x701b, 0x90ad, 0x701c, 0x806e, 0x701d, 0x90ae,
0x701e, 0x806f, 0x701f, 0x90af, 0x7020, 0x8070, 0x7021, 0x90b0, 0x7022, 0x8071, 0x7023, 0x90b1, 0x7024, 0x8072, 0x7025, 0x90b2,
0x7026, 0x8073, 0x7027, 0x90b3, 0x7028, 0x8074, 0x7029, 0x90b4, 0x702a, 0x8075, 0x702b, 0x90b5, 0x702c, 0x8076, 0x702d, 0x90b6,
0x702e, 0x8077, 0x702f, 0x90b7, 0x7030, 0x8078, 0x7031, 0x90b8, 0x7032, 0x8079, 0x7033, 0x90b9, 0x7034, 0x807a, 0x7035, 0x90ba,
0x7036, 0x807b, 0x7037, 0x90bb, 0x7038, 0x807c, 0x7039, 0x90bc, 0x703a, 0x807d, 0x703b, 0x90bd, 0x703c, 0x807e, 0x703d, 0x90be,
0x703e, 0x807f, 0x703f, 0x90bf, 0x7000, 0x8040, 0x7001, 0x90c0, 0x7002, 0x8041, 0x7003, 0x90c1, 0x7004, 0x8042, 0x7005, 0x90c2,
0x7006, 0x8043, 0x7007, 0x90c3, 0x7008, 0x8044, 0x7009, 0x90c4, 0x700a, 0x8045, 0x700b, 0x90c5, 0x700c, 0x8046, 0x700d, 0x90c6,
0x700e, 0x8047, 0x700f, 0x90c7, 0x7010, 0x8048, 0x7011, 0x90c8, 0x7012, 0x8049, 0x7013, 0x90c9, 0x7014, 0x804a, 0x7015, 0x90ca,
0x7016, 0x804b, 0x7017, 0x90cb, 0x7018, 0x804c, 0x7019, 0x90cc, 0x701a, 0x804d, 0x701b, 0x90cd, 0x701c, 0x804e, 0x701d, 0x90ce,
0x701e, 0x804f, 0x701f, 0x90cf, 0x7020, 0x8050, 0x7021, 0x90d0, 0x7022, 0x8051, 0x7023, 0x90d1, 0x7024, 0x8052, 0x7025, 0x90d2,
0x7026, 0x8053, 0x7027, 0x90d3, 0x7028, 0x8054, 0x7029, 0x90d4, 0x702a, 0x8055, 0x702b, 0x90d5, 0x702c, 0x8056, 0x702d, 0x90d6,
0x702e, 0x8057, 0x702f, 0x90d7, 0x7030, 0x8058, 0x7031, 0x90d8, 0x7032, 0x8059, 0x7033, 0x90d9, 0x7034, 0x805a, 0x7035, 0x90da,
0x7036, 0x805b, 0x7037, 0x90db, 0x7038, 0x805c, 0x7039, 0x90dc, 0x703a, 0x805d, 0x703b, 0x90dd, 0x703c, 0x805e, 0x703d, 0x90de,
0x703e, 0x805f, 0x703f, 0x90df, 0x7000, 0x8060, 0x7001, 0x90e0, 0x7002, 0x8061, 0x7003, 0x90e1, 0x7004, 0x8062, 0x7005, 0x90e2,
0x7006, 0x8063, 0x7007, 0x90e3, 0x7008, 0x8064, 0x7009, 0x90e4, 0x700a, 0x8065, 0x700b, 0x90e5, 0x700c, 0x8066, 0x700d, 0x90e6,
0x700e, 0x8067, 0x700f, 0x90e7, 0x7010, 0x8068, 0x7011, 0x90e8, 0x7012, 0x8069, 0x7013, 0x90e9, 0x7014, 0x806a, 0x7015, 0x90ea,
0x7016, 0x806b, 0x7017, 0x90eb, 0x7018, 0x806c, 0x7019, 0x90ec, 0x701a, 0x806d, 0x701b, 0x90ed, 0x701c, 0x806e, 0x701d, 0x90ee,
0x701e, 0x806f, 0x701f, 0x90ef, 0x7020, 0x8070, 0x7021, 0x90f0, 0x7022, 0x8071, 0x7023, 0x90f1, 0x7024, 0x8072, 0x7025, 0x90f2,
0x7026, 0x8073, 0x7027, 0x90f3, 0x7028, 0x8074, 0x7029, 0x90f4, 0x702a, 0x8075, 0x702b, 0x90f5, 0x702c, 0x8076, 0x702d, 0x90f6,
0x702e, 0x8077, 0x702f, 0x90f7, 0x7030, 0x8078, 0x7031, 0x90f8, 0x7032, 0x8079, 0x7033, 0x90f9, 0x7034, 0x807a, 0x7035, 0x90fa,
0x7036, 0x807b, 0x7037, 0x90fb, 0x7038, 0x807c, 0x7039, 0x90fc, 0x703a, 0x807d, 0x703b, 0x90fd, 0x703c, 0x807e, 0x703d, 0x90fe,
0x703e, 0x807f, 0x703f, 0x90ff };

static const uint16_t* DRG[] = { drg0, drg1, drg2, drg3, drg4, drg5, drg6, drg7 };

// rung 1 and 2 double value decoding tables, can use 8 bits
static const uint8_t DDRG1[] = { 0x20, 0x31, 0x34, 0x42, 0x20, 0x45, 0x48, 0x43, 0x20, 0x31, 0x34, 0x56, 0x20, 0x59, 0x4c,
0x57, 0x20, 0x31, 0x34, 0x42, 0x20, 0x45, 0x48, 0x43, 0x20, 0x31, 0x34, 0x6a, 0x20, 0x5d, 0x4c, 0x6b, 0x20, 0x31, 0x34, 0x42,
0x20, 0x45, 0x48, 0x43, 0x20, 0x31, 0x34, 0x56, 0x20, 0x59, 0x4c, 0x57, 0x20, 0x31, 0x34, 0x42, 0x20, 0x45, 0x48, 0x43, 0x20,
0x31, 0x34, 0x6e, 0x20, 0x5d, 0x4c, 0x6f };
static const uint16_t DDRG2[] = { 0x4000, 0x5002, 0x4001, 0x6004, 0x5010, 0x5003, 0x5011, 0x6005, 0x4008, 0x6012, 0x4009,
0x6006, 0x6020, 0x6013, 0x6021, 0x6007, 0x4000, 0x500a, 0x4001, 0x7014, 0x5018, 0x500b, 0x5019, 0x7015, 0x4008, 0x7022, 0x4009,
0x7016, 0x6028, 0x7023, 0x6029, 0x7017, 0x4000, 0x5002, 0x4001, 0x600c, 0x5010, 0x5003, 0x5011, 0x600d, 0x4008, 0x601a, 0x4009,
0x600e, 0x6030, 0x601b, 0x6031, 0x600f, 0x4000, 0x500a, 0x4001, 0x8024, 0x5018, 0x500b, 0x5019, 0x8025, 0x4008, 0x702a, 0x4009,
0x8026, 0x6038, 0x702b, 0x6039, 0x8027, 0x4000, 0x5002, 0x4001, 0x6004, 0x5010, 0x5003, 0x5011, 0x6005, 0x4008, 0x6012, 0x4009,
0x6006, 0x6020, 0x6013, 0x6021, 0x6007, 0x4000, 0x500a, 0x4001, 0x701c, 0x5018, 0x500b, 0x5019, 0x701d, 0x4008, 0x7032, 0x4009,
0x701e, 0x6028, 0x7033, 0x6029, 0x701f, 0x4000, 0x5002, 0x4001, 0x600c, 0x5010, 0x5003, 0x5011, 0x600d, 0x4008, 0x601a, 0x4009,
0x600e, 0x6030, 0x601b, 0x6031, 0x600f, 0x4000, 0x500a, 0x4001, 0x802c, 0x5018, 0x500b, 0x5019, 0x802d, 0x4008, 0x703a, 0x4009,
0x802e, 0x6038, 0x703b, 0x6039, 0x802f, 0x4000, 0x5002, 0x4001, 0x6004, 0x5010, 0x5003, 0x5011, 0x6005, 0x4008, 0x6012, 0x4009,
0x6006, 0x6020, 0x6013, 0x6021, 0x6007, 0x4000, 0x500a, 0x4001, 0x7014, 0x5018, 0x500b, 0x5019, 0x7015, 0x4008, 0x7022, 0x4009,
0x7016, 0x6028, 0x7023, 0x6029, 0x7017, 0x4000, 0x5002, 0x4001, 0x600c, 0x5010, 0x5003, 0x5011, 0x600d, 0x4008, 0x601a, 0x4009,
0x600e, 0x6030, 0x601b, 0x6031, 0x600f, 0x4000, 0x500a, 0x4001, 0x8034, 0x5018, 0x500b, 0x5019, 0x8035, 0x4008, 0x702a, 0x4009,
0x8036, 0x6038, 0x702b, 0x6039, 0x8037, 0x4000, 0x5002, 0x4001, 0x6004, 0x5010, 0x5003, 0x5011, 0x6005, 0x4008, 0x6012, 0x4009,
0x6006, 0x6020, 0x6013, 0x6021, 0x6007, 0x4000, 0x500a, 0x4001, 0x701c, 0x5018, 0x500b, 0x5019, 0x701d, 0x4008, 0x7032, 0x4009,
0x701e, 0x6028, 0x7033, 0x6029, 0x701f, 0x4000, 0x5002, 0x4001, 0x600c, 0x5010, 0x5003, 0x5011, 0x600d, 0x4008, 0x601a, 0x4009,
0x600e, 0x6030, 0x601b, 0x6031, 0x600f, 0x4000, 0x500a, 0x4001, 0x803c, 0x5018, 0x500b, 0x5019, 0x803d, 0x4008, 0x703a, 0x4009,
0x803e, 0x6038, 0x703b, 0x6039, 0x803f };
// rung 3 double value would be 2k by itself, the normal one is 64 bytes, and it gets worse from there

// Decoding tables for codeswitch
static const uint16_t dsw3[] = { 0x3001, 0x4002, 0x3007, 0x5003, 0x3001, 0x4006, 0x3007, 0x5005, 0x3001, 0x4002, 0x3007, 0x5000,
0x3001, 0x4006, 0x3007, 0x5004 };
static const uint16_t dsw4[] = { 0x4001, 0x5003, 0x400f, 0x6005, 0x4002, 0x500d, 0x400e, 0x600b, 0x4001, 0x5004, 0x400f, 0x6006,
0x4002, 0x500c, 0x400e, 0x600a, 0x4001, 0x5003, 0x400f, 0x6007, 0x4002, 0x500d, 0x400e, 0x6009, 0x4001, 0x5004, 0x400f, 0x6000,
0x4002, 0x500c, 0x400e, 0x6008 };
static const uint16_t dsw5[] = { 0x5001, 0x6005, 0x501f, 0x7009, 0x5002, 0x601b, 0x501e, 0x7017, 0x5003, 0x6006, 0x501d, 0x700a,
0x5004, 0x601a, 0x501c, 0x7016, 0x5001, 0x6007, 0x501f, 0x700b, 0x5002, 0x6019, 0x501e, 0x7015, 0x5003, 0x6008, 0x501d, 0x700c,
0x5004, 0x6018, 0x501c, 0x7014, 0x5001, 0x6005, 0x501f, 0x700d, 0x5002, 0x601b, 0x501e, 0x7013, 0x5003, 0x6006, 0x501d, 0x700e,
0x5004, 0x601a, 0x501c, 0x7012, 0x5001, 0x6007, 0x501f, 0x700f, 0x5002, 0x6019, 0x501e, 0x7011, 0x5003, 0x6008, 0x501d, 0x7000,
0x5004, 0x6018, 0x501c, 0x7010 };
static const uint16_t dsw6[] = { 0x6001, 0x7009, 0x603f, 0x8011, 0x6002, 0x7037, 0x603e, 0x802f, 0x6003, 0x700a, 0x603d, 0x8012,
0x6004, 0x7036, 0x603c, 0x802e, 0x6005, 0x700b, 0x603b, 0x8013, 0x6006, 0x7035, 0x603a, 0x802d, 0x6007, 0x700c, 0x6039, 0x8014,
0x6008, 0x7034, 0x6038, 0x802c, 0x6001, 0x700d, 0x603f, 0x8015, 0x6002, 0x7033, 0x603e, 0x802b, 0x6003, 0x700e, 0x603d, 0x8016,
0x6004, 0x7032, 0x603c, 0x802a, 0x6005, 0x700f, 0x603b, 0x8017, 0x6006, 0x7031, 0x603a, 0x8029, 0x6007, 0x7010, 0x6039, 0x8018,
0x6008, 0x7030, 0x6038, 0x8028, 0x6001, 0x7009, 0x603f, 0x8019, 0x6002, 0x7037, 0x603e, 0x8027, 0x6003, 0x700a, 0x603d, 0x801a,
0x6004, 0x7036, 0x603c, 0x8026, 0x6005, 0x700b, 0x603b, 0x801b, 0x6006, 0x7035, 0x603a, 0x8025, 0x6007, 0x700c, 0x6039, 0x801c,
0x6008, 0x7034, 0x6038, 0x8024, 0x6001, 0x700d, 0x603f, 0x801d, 0x6002, 0x7033, 0x603e, 0x8023, 0x6003, 0x700e, 0x603d, 0x801e,
0x6004, 0x7032, 0x603c, 0x8022, 0x6005, 0x700f, 0x603b, 0x801f, 0x6006, 0x7031, 0x603a, 0x8021, 0x6007, 0x7010, 0x6039, 0x8000,
0x6008, 0x7030, 0x6038, 0x8020 };

// integer mag-sign to normal encoding without conditionals
template<typename T> static T smag(T v) { return (v >> 1) ^ (~T(0) * (v & 1)); }

// Computed decode, does not work for rung 0 or 1
static std::pair<size_t, uint64_t> qb3dsz(uint64_t val, size_t rung) {
    assert(rung > 1);
    uint64_t rbit = 1ull << rung;
    if (0 == (val & 1)) // Short
        return std::make_pair(rung, (val & (rbit - 1)) >> 1);
    uint64_t n = (val >> 1) & 1; // Next bit, long if set
    val = (val >> 2) & (rbit - 1);
    return std::make_pair(rung + 1 + n,
        (((1 & ~n) * ~0ull)  & (val | (rbit >> 1))) // Nominal
        + (((1 & n) * ~0ull) & (val | rbit)));    // Long
}

// Decode using tables when possible, works for all rungs
static std::pair<size_t, uint64_t> qb3dsztbl(uint64_t val, size_t rung) {
    if ((sizeof(DRG) / sizeof(*DRG)) > rung) {
        auto code = DRG[rung][val & ((1ull << (rung + 2)) - 1)];
        return std::make_pair<size_t, uint64_t>(code >> 12, code & TBLMASK);
    }
    return qb3dsz(val, rung);
}

// Decode a B2 sized group of QB3 values from s and acc
// Accumulator should be valid and have at least 56 valid bits
// For rung 0, it works with 17bits or more
// For rung 1, it works with 47bits or more
// returns false on failure
template<typename T>
static bool gdecode(iBits& s, size_t rung, T* group, uint64_t acc, size_t abits) {
    assert(((rung > 1) && (abits <= 8))
        || ((rung == 1) && (abits <= 17)) // B2 + 1
        || ((rung == 0) && (abits <= 47))); // 3 * B2 - 1
    if (0 == rung) { // single bits, direct decoding
        if (0 != (acc & 1)) {
            abits += B2;
            for (size_t i = 0; i < B2; i++) {
                acc >>= 1;
                group[i] = static_cast<T>(1 & acc);
            }
        }
        else
            for (size_t i = 0; i < B2; i++)
                group[i] = static_cast<T>(0);
        s.advance(abits + 1);
        return 1;
    }
    // Table decoding
    if (sizeof(T) == 1 || rung < (sizeof(DRG) / sizeof(*DRG))) {
        if (1 == rung) { // double barrel
            for (size_t i = 0; i < B2; i += 2) {
                auto v = DDRG1[acc & 0x3f];
                group[i] = v & 0x3;
                group[i + 1] = (v >> 2) & 0x3;
                abits += v >> 4;
                acc >>= v >> 4;
            }
            s.advance(abits);
        }
        else if (2 == rung) { // double barrel, max sym len is 4, there are at least 14 in the accumulator
            for (size_t i = 0; i < 14; i += 2) {
                auto v = DDRG2[acc & 0xff];
                group[i] = v & 0x7;
                group[i + 1] = (v >> 3) & 0x7;
                abits += v >> 12;
                acc >>= v >> 12;
            }
            if (abits > 56) { // Rare
                s.advance(abits);
                acc = s.peek();
                abits = 0;
            }
            // last pair
            auto v = DDRG2[acc & 0xff];
            group[14] = v & 0x7;
            group[15] = (v >> 3) & 0x7;
            s.advance(abits + (v >> 12));
        }
        else if (6 > rung) { // Table decode at 3,4 and 5, half of the values per accumulator
            auto drg = DRG[rung];
            const auto m = (1ull << (rung + 2)) - 1;
            for (size_t i = 0; i < B2 / 2; i++) {
                auto v = drg[acc & m];
                abits += v >> 12;
                acc >>= v >> 12;
                group[i] = static_cast<T>(v & TBLMASK);
            }
            s.advance(abits);
            acc = s.peek();
            abits = 0;
            for (size_t i = B2 / 2; i < B2; i++) {
                auto v = drg[acc & m];
                abits += v >> 12;
                acc >>= v >> 12;
                group[i] = static_cast<T>(v & TBLMASK);
            }
            s.advance(abits);
        }
        else { // Last part of table decoding, rungs 6-7, four values per accumulator
            auto drg = DRG[rung];
            const auto m = (1ull << (rung + 2)) - 1;
            for (size_t j = 0; j < B2; j += B2 / 4) {
                for (size_t i = 0; i < B2 / 4; i++) {
                    auto v = drg[acc & m];
                    abits += v >> 12;
                    acc >>= v >> 12;
                    group[j + i] = static_cast<T>(v & TBLMASK);
                }
                s.advance(abits);
                abits = 0;
                if (j <= B2 / 2) // Skip the last peek
                    acc = s.peek();
            }
        }
    }
    else { // computed decoding
        if (sizeof(T) < 8 || rung < 32) { // 16 and 32 bits may reuse accumulator
            for (int i = 0; i < B2; i++) {
                if (abits + rung > 62) {
                    s.advance(abits);
                    acc = s.peek();
                    abits = 0;
                }
                auto p = qb3dsz(acc, rung);
                abits += p.first;
                acc >>= p.first;
                group[i] = static_cast<T>(p.second);
            }
            s.advance(abits);
        }
        else if (rung < 63) { // 64bit and rung in [32 - 62], can't reuse accumulator
            s.advance(abits);
            for (int i = 0; i < B2; i++) {
                auto p = qb3dsz(s.peek(), rung);
                group[i] = static_cast<T>(p.second);
                s.advance(p.first);
            }
        }
        else { // Rung 63 might need 65 bits
            s.advance(abits);
            for (int i = 0; i < B2; i++) {
                auto p = qb3dsz(s.peek(), rung);
                auto ovf = p.first & (p.first >> 6);
                group[i] = static_cast<T>(p.second);
                s.advance(p.first ^ ovf);
                if (ovf) // The next to top bit got dropped, rare
                    group[i] |= s.get() << 62;
            }
        }
    }
    if (0 == (group[B2 - 1] >> rung)) {
        auto stepp = step(group, rung);
        if (stepp < B2)
            group[stepp] ^= static_cast<T>(1ull << rung);
    }
    return true;
}

// Absolute from mag-sign
template<typename T> static T magsabs(T v) { return (v >> 1) + (v & 1); }

// Multiply v(in magsign) by m(normal, positive)
template<typename T> static T magsmul(T v, T m) { return magsabs(v) * (m << 1) - (v & 1); }

// reports most but not all errors, for example if the input stream is too short for the last block
template<typename T>
static bool decode(uint8_t *src, size_t len, T* image, 
    size_t xsize, size_t ysize, size_t bands, uint8_t *cband)
{
    static_assert(std::is_integral<T>() && std::is_unsigned<T>(), "Only unsigned integer types allowed");
    // Best block traversal order in most cases
    const uint8_t xlut[16] = { 0, 1, 0, 1, 2, 3, 2, 3, 0, 1, 0, 1, 2, 3, 2, 3 };
    const uint8_t ylut[16] = { 0, 0, 1, 1, 0, 0, 1, 1, 2, 2, 3, 3, 2, 2, 3, 3 };
    constexpr size_t UBITS(sizeof(T) == 1 ? 3 : sizeof(T) == 2 ? 4 : sizeof(T) == 4 ? 5 : 6);
    constexpr auto NORM_MASK((1ull << UBITS) - 1); // UBITS set
    constexpr auto LONG_MASK(NORM_MASK * 2 + 1); // UBITS + 1 set
    T prev[QB3_MAXBANDS] = {}, pcf[QB3_MAXBANDS] = {}, group[B2] = {};
    size_t runbits[QB3_MAXBANDS] = {}, offset[B2] = {};
    const uint16_t* dsw = sizeof(T) == 1 ? dsw3 : sizeof(T) == 2 ? dsw4 : sizeof(T) == 4 ? dsw5 : dsw6;
    for (size_t i = 0; i < B2; i++)
        offset[i] = (xsize * ylut[i] + xlut[i]) * bands;
    iBits s(src, len);

    bool failed(false);
    for (size_t y = 0; y < ysize; y += B) {
        // If the last row is partial, roll it up
        if (y + B > ysize)
            y = ysize - B;
        for (size_t x = 0; x < xsize; x += B) {
            // If the last column is partial, move it left
            if (x + B > xsize)
                x = xsize - B;
            for (int c = 0; c < bands; c++) {
                failed |= s.empty();
                uint64_t cs(0), abits(1), acc(s.peek());
                if (acc & 1) { // Rung change
                    cs = dsw[(acc >> 1) & LONG_MASK];
                    abits = cs >> 12;
                }
                acc >>= abits;
                if (0 == cs || 0 != (cs & TBLMASK)) { // Normal decoding, not a signal
                    // abits is never > 8, so it's safe to call gdecode
                    auto rung = (runbits[c] + cs) & NORM_MASK;
                    failed |= !gdecode(s, rung, group, acc, abits);
                    runbits[c] = rung;
                }
                else { // extra encoding
                    cs = dsw[acc & LONG_MASK]; // rung, no flag
                    auto rung = (runbits[c] + cs) & NORM_MASK;
                    acc >>= (cs >> 12) - 1; // No flag
                    abits += (cs >> 12) - 1;
                    if (rung != NORM_MASK) { // CF encoding
                        auto cfrung(rung);
                        T cf = pcf[c];
                        auto read_cfr = acc & 1;
                        abits++;
                        acc >>= 1;
                        if (read_cfr) { // different cf, need to read it
                            read_cfr = acc & 1;
                            abits++;
                            acc >>= 1;
                            if (read_cfr) { // has own rung
                                cs = dsw[acc & LONG_MASK];
                                cfrung = (rung + cs) & NORM_MASK;
                                failed |= (cfrung == rung);
                                acc >>= (cs >> 12) - 1;
                                abits += (cs >> 12) - 1;
                            }
                            if (sizeof(T) == 8 && (cfrung + abits) > 62) { // Rare
                                s.advance(abits);
                                acc = s.peek();
                                abits = 0;
                            }
                            auto p = qb3dsztbl(acc, cfrung - read_cfr);
                            pcf[c] = cf = static_cast<T>(p.second + (read_cfr << cfrung));
                            abits += p.first;
                            acc >>= p.first;
                        }
                        cf += 2; // Use it unbiased
                        if (rung) {
                            s.advance(abits);
                            failed |= !gdecode(s, rung, group, s.peek(), 0);
                            // Multiply group by CF and get the max for the actual rung
                            T maxval(group[0] = magsmul(group[0], cf));
                            for (int i = 1; i < B2; i++) {
                                auto val = magsmul(group[i], cf);
                                if (maxval < val) maxval = val;
                                group[i] = val;
                            }
                            failed |= cf > maxval; // Can't be all zero
                            runbits[c] = topbit(maxval | 1);
                        }
                        else { // Single bit for data, decode here
                            if (abits + B2 > 64) {
                                s.advance(abits);
                                acc = s.peek();
                                abits = 0;
                            }
                            T v[2] = { 0, magsmul(T(1), cf) };
                            for (int i = 0; i < B2; i++)
                                group[i] = v[(acc >> i) & 1];
                            s.advance(B2 + abits);
                            runbits[c] = topbit(v[1]);
                        }
                    }
                    else { // IDX decoding
                        cs = dsw[acc & LONG_MASK]; // rung, no flag
                        rung = (runbits[c] + cs) & NORM_MASK;
                        runbits[c] = rung;
                        acc >>= (cs >> 12) - 1; // No flag
                        abits += (cs >> 12) - 1;
                        failed |= rung == 63; // TODO: Deal with 64bit overflow
                        // 16 index values in group, max is 7
                        T maxval(0);
                        for (int i = 0; i < B2; i++) {
                            // Could use ddrg2
                            auto v = DRG[2][acc & 0xf];
                            group[i] = static_cast<uint8_t>(v);
                            if (maxval < group[i])
                                maxval = group[i];
                            acc >>= v >> 12;
                            abits += v >> 12;
                        }
                        s.advance(abits);
                        T idxarray[B2 / 2] = {};
                        for (size_t i = 0; i <= maxval; i++) {
                            acc = s.peek();
                            auto v = qb3dsztbl(acc, rung);
                            s.advance(v.first);
                            idxarray[i] = T(v.second);
                        }
                        for (int i = 0; i < B2; i++)
                            group[i] = idxarray[group[i]];
                    }
                }
                // Undo delta encoding for this block
                auto prv = prev[c];
                T* const blockp = image + (y * xsize + x) * bands + c;
                for (int i = 0; i < B2; i++)
                    blockp[offset[i]] = prv += smag(group[i]);
                prev[c] = prv;
            } // Per band per block
            if (failed) break;
        } // per block
        if (failed) break;
        // For performance apply band delta per block stip, in linear order
        for (int c = 0; c < bands; c++) if (c != cband[c]) {
            auto dimg = image + bands * xsize * y + c;
            auto simg = image + bands * xsize * y + cband[c];
            for (int i = 0; i < B * xsize; i++, dimg += bands, simg += bands)
                *dimg += *simg;
        }
    } // per block strip
    // It might not catch all errors
    return failed || s.avail() > 7; 
}
} // namespace
