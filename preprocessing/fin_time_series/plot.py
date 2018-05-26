# -*- coding: utf-8 -*-
"""
Created on Thu Apr 12 11:48:41 2018

@author: Vinko Kolobara
"""

import pandas as pd
from financial_time_series import *
import random

from matplotlib import pyplot as plt

df = pd.read_csv("../data/btcusd_6h_prep.csv")
df_test = pd.read_csv("../data/btcusd_6h_prep_test.csv")


balance = 100.0
bought = 0

vals = df['pred'].values
vals_test = df_test['pred'].values

'''
# BUY LOW SELL HIGH #########################################################
bought = [22675, 22677, 22678, 22681, 22687, 22689, 22690, 22691, 22692, 22699, 22718, 22729, 24118, 24131, 24132, 24611, 24765, 24877, 24878, 24882, 26881, 26915, 26938, 27279, 27696, 29416, 31352, 31681, 31682, 31777, 31786, 31789, 31855, 32049, 32604, 33340, 33346, 36931, 37071, 37151, 40333, 41002, 41015, 41320, 42216, 43135, 45327, 46031, 51343, 52401, 54111, 54305, 54602, 54603, 54604, 62932, 62949, 67293, 67504, 67906, 67919, 67923, 67929, 67930, 67942, 68042, 68610, 68804, 119237, 119239, 119240, 119611, 119612, 119613, 119616, 119620, 119636, 119637, 119642, 120558, 120595, 121747, 122513]
sold = [79226, 79639, 80508, 80509, 80555, 80654, 80756, 80766, 81249, 81253, 81446, 81449, 81508, 81511, 81512, 81550, 81684, 82204, 82205, 82324, 88336, 93027, 93044, 93047, 93117, 93134, 93135, 93145, 93153, 93156, 93169, 93246, 93249, 93272, 93281, 93282, 93295, 93394, 93401, 93548, 93557, 93657, 93658, 93835, 93880, 93939, 93954, 93957, 93960, 93987, 93989, 94064, 94245, 94285, 94291, 94295, 94304, 94314, 94315, 94321, 94322, 94325, 94327, 94328, 94329, 94337, 94341, 94342]

print(len(bought))
print(len(sold))
print()

plt.figure()
plt.title("69%")
plt.plot(vals)
plt.plot(bought, vals[bought], 'og')
plt.plot(sold, vals[sold], 'or')

# BUY LOW SELL HIGH
bought = [22675, 22677, 22678, 22681, 22687, 22689, 22690, 22691, 22692, 22699, 22718, 22729, 24118, 24131, 24132, 24611, 24765, 24877, 24878, 24882, 26881, 26915, 26938, 27279, 27696, 29416, 31352, 31681, 31682, 31777, 31786, 31789, 31855, 32049, 32604, 33340, 33346, 36931, 37071, 37151, 40333, 41002, 41015, 41320, 42216, 43135, 45327, 46031, 51343, 52401, 54111, 54305, 54602, 54603, 54604, 62932, 62949, 67293, 67504, 67906, 67919, 67923, 67929, 67930, 67942, 68042, 68610, 68804, 119237, 119239, 119240, 119611, 119612, 119613, 119616, 119620, 119636, 119637, 119642, 120558, 120595, 121747, 122513]
sold = [80776, 80893, 81363, 81366, 82222, 82227, 82228, 82229, 82290, 82826, 93205, 93762, 93820, 93823, 93824, 94524, 94557, 94914, 94940, 95120, 95129, 95130, 95132, 95325, 95353, 95359, 95421, 95444, 95564, 95565, 95567, 95580, 95662, 96422, 96587, 97099, 97941, 97979, 97989, 97990, 97993, 97999, 98010, 98012, 98115, 98122, 98123, 98141, 98143, 98148, 98150, 98284, 98353, 98354, 98575, 99082, 99173, 99184, 99185, 99280, 99281, 100285, 100850, 101247, 101848, 103913, 103914, 103930]

print(len(bought))
print(len(sold))
print()

plt.figure()
plt.title("70%")
plt.plot(vals)
plt.plot(bought, vals[bought], 'og')
plt.plot(sold, vals[sold], 'or')
'''

''' bought = [2274, 2284, 3089, 3142, 3263, 3434, 4251, 4548, 8088, 8549, 10251, 14291, 21534, 22541, 22544, 22561, 22639, 22673, 22675, 22687, 22691, 22717, 22718, 22723, 23329, 24131, 24611, 24723, 25484, 27695, 31682, 31848, 32049, 33358, 33895, 37071, 38704, 38706, 41320, 42196, 42755, 45327, 45697, 50040, 50246, 50312, 52400, 54111, 58061, 58364, 59749, 59921, 66827, 67504, 67919, 67923, 67934, 67935, 68042, 69598, 70678, 71485, 71572, 71800, 73212, 73213, 73397, 74682, 79264, 79408, 79659, 79666, 82846, 82948, 82993, 83440, 83467, 83634, 83641, 84178, 84190, 84210, 84225, 86303, 86640, 94557, 94914, 95421, 98148, 99281, 99560, 101247, 104695, 104707, 104820, 105319, 106514, 106591, 108108, 111433, 112366, 113163, 113224, 117217, 119620]
sold = [10742, 10750, 13403, 20937, 22679, 22692, 22693, 22694, 24616, 31352, 46031, 50215, 82856, 82857, 83046, 83050, 83457, 83462, 90817, 90818, 90819, 90821, 90824, 95567, 96374, 96376, 97987, 98012, 98013, 98015, 99038, 99282, 103913, 103914, 103915, 104649, 104656, 104661, 104665, 104697, 104698, 104699, 104700, 104701, 104702, 104717, 104718, 105320, 105321, 105322, 105924, 105947, 110216, 111435, 111440, 113148, 113150, 113151, 113152, 113153, 113157, 113159, 119180, 119612, 119614, 119616]

print(len(bought))
print(len(sold))
print()

plt.figure()
plt.title("Balance: 111.727")
plt.plot(vals)
plt.plot(bought, vals[bought], 'og')
plt.plot(sold, vals[sold], 'or')


bought = [2274, 2284, 3089, 3142, 3263, 3434, 4251, 4548, 8088, 8549, 10251, 14291, 21534, 22541, 22544, 22561, 22639, 22673, 22675, 22678, 22687, 22691, 22717, 22718, 22723, 23329, 24118, 24131, 24611, 24723, 25484, 27695, 31682, 31848, 32049, 33358, 33895, 37071, 38704, 38706, 41320, 42196, 42755, 45327, 45697, 50040, 50246, 50312, 52400, 54111, 58061, 58364, 59749, 59921, 62932, 66827, 67504, 67919, 67923, 67934, 67935, 68042, 69598, 70678, 71485, 71572, 71800, 73212, 73213, 73397, 74682, 79264, 79408, 79659, 79666, 82846, 82948, 82993, 83440, 83467, 83634, 83641, 84178, 84190, 84210, 84225, 94557, 94914, 95421, 98148, 99281, 99560, 101247, 104695, 104707, 104820, 105319, 106514, 106591, 108108, 111433, 112366, 113163, 113224, 117217, 119611, 119620]
sold = [10742, 10750, 13403, 20937, 22679, 22692, 22693, 22694, 24616, 31352, 46031, 50215, 82856, 82857, 83046, 83050, 83457, 83462, 90817, 90818, 90819, 90821, 90824, 95567, 96374, 96376, 97987, 98012, 98013, 98015, 99038, 99282, 103913, 103914, 103915, 104649, 104656, 104661, 104665, 104697, 104698, 104699, 104700, 104701, 104702, 104717, 104718, 105320, 105321, 105322, 105924, 105947, 110216, 111435, 111440, 113148, 113150, 113151, 113152, 113153, 113157, 113159, 119180, 119612, 119614, 119616]

print(len(bought))
print(len(sold))
print()

plt.figure()
plt.title("BALANCE: 113.413")
plt.plot(vals)
plt.plot(bought, vals[bought], 'og')
plt.plot(sold, vals[sold], 'or') '''

bought = [16, 17, 18, 21, 22, 23, 24, 78, 103, 108, 120, 138, 172, 189, 190, 199, 212]
sold = [76, 87, 107, 119, 137, 171, 187, 188, 197, 211]

plt.figure()
plt.plot(vals_test)
plt.plot(bought, vals_test[bought], 'og')
plt.plot(sold, vals_test[sold], 'or')

bought = [4, 12, 14, 20, 26, 39, 43, 45, 47, 51, 73, 77, 81, 85, 89, 227, 306, 308, 310, 316, 317, 319, 333, 334, 367, 374, 376, 377, 382, 385, 387, 388, 389, 390, 399, 401, 406, 407, 444, 446, 448, 473, 480, 481, 482, 483, 500, 505, 512, 513, 514, 515, 516, 520, 522, 528, 535, 536, 537, 538, 539, 540, 541, 542, 543, 546, 547, 548, 549, 550, 552, 561, 562, 565, 566]
sold = [2, 24, 29, 68, 75, 79, 84, 87, 98, 99, 118, 124, 130, 133, 134, 136, 142, 149, 151, 158, 160, 167, 168, 169, 171, 172, 178, 179, 182, 183, 184, 186, 187, 191, 192, 194, 196, 199, 201, 202, 217, 219, 220, 224, 238, 240, 241, 242, 245, 246, 247, 248, 249, 253, 254, 255, 256, 257, 259, 301, 304, 337, 354, 366, 368, 369, 436, 437, 443, 445, 449, 529, 530, 531, 563]

plt.figure()
plt.plot(vals)
plt.plot(bought, vals[bought], 'og')
plt.plot(sold, vals[sold], 'or')
plt.show()