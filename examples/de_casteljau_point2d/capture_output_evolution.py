import matplotlib.pyplot as mpl
import numpy as np
import subprocess

#try:
#    logfilename
#except NameError: logfilename = '/tmp/de_casteljau_subdivide_output.txt'
#
#logfile = open(logfilename, 'w')

dc_input_first = None
dc_input_last = None
dc_output_first = None
dc_output_last = None
dc_input = []
dc_output = []
dc_t = 0.0

def helper(first, last, output_list):
    while first < last:
        val = first.dereference()
        output_list.append([float(val['data'][0]), float(val['data'][1])])
        first += 1

class Custombreakpoint(gdb.Breakpoint):
    def __init__(self, function, on_entry):
        self.on_entry = on_entry
        super().__init__(function, internal=True)

    def stop(self):
        if self.on_entry:
            global dc_input_first
            dc_input_first = gdb.parse_and_eval("&(*first)")
            global dc_input_last
            dc_input_last = gdb.parse_and_eval("&(*last)")
            global dc_output_first
            dc_output_first = gdb.parse_and_eval("&(*d_first)")
            global dc_output_last
            dc_output_last = gdb.parse_and_eval("&(*d_last)") + 1
            global dc_t
            dc_t = float(gdb.parse_and_eval("t"))
            helper(dc_input_first, dc_input_last, dc_input)
        foo = []
        helper(dc_output_first, dc_output_last, foo)
        dc_output.append(foo)
        return False

args = ["rg", "-no", "SPLINE_TRACE.*",
        "include/spline/de_casteljau_subdivide.hpp"]
result = subprocess.run(args, stdout=subprocess.PIPE)

lines_to_break_on = []
for line in result.stdout.decode('utf-8').splitlines():
    lines_to_break_on.append(int(line.split(':')[0]))

# First breakpoint is special, we need to capture initial d_first
Custombreakpoint('de_casteljau_subdivide.hpp:{}'.format(lines_to_break_on[0]), on_entry=True)
for line in lines_to_break_on[1:]:
    Custombreakpoint('de_casteljau_subdivide.hpp:{}'.format(line), on_entry=False)

gdb.execute('run')

input_ar = np.array(dc_input)
mpl.plot(input_ar[:, 0], input_ar[:,1], 'ro-')
N = input_ar.shape[0]
for i, row in enumerate(dc_output[1:]):
    output = np.array(row[(i + 1):-(i + 1)])
    b = np.isnan(output) == False
    if i == N - 2:
        mpl.plot(output[b[:, 0], 0], output[b[:, 1], 1], 'bx')
    else:
        mpl.plot(output[b[:, 0], 0], output[b[:, 1], 1])

mpl.show()

gdb.execute('quit')

