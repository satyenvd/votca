#!/usr/bin/env python3
#
# Copyright 2009-2023 The VOTCA Development Team (http://www.votca.org)
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#


import argparse
import sys
import re
import pickle
import os

class state:
    """state class"""

    def __init__(self):
        self.state = "Undefined"
        self.parameters = []
        self.solutions = []
        self.comments = ""

    def read(self, filename):
        statefile = open(filename)
        for line in statefile:
            m = re.match("^#State = \s*(\S*)", line)
            if m:
                self.state = m.group(1)
            elif re.match("^#", line):
                self.comments += line
            else:
                if len(line.strip()) == 0:
                    continue
                try:
                    li = line.split()
                    array = numpy.array([numpy.float64(i)
                                         for i in line.split()[0:-1]])
                except:
                    sys.exit("paramter set ("+line.strip() +
                             ") contains a non-numerical value")
                self.parameters.append(array[0:-1])
                self.solutions.append(array[-1])
                if self.state != "Initialization" and not re.match("^(complete|try)$", li[-1]):
                    sys.exit(
                        "We can only handle parameter sets flagged with Complete or Try and we found '"+li[-1]+"'")
        statefile.close()
        self.comments = self.comments.strip()
        if self.state == "Undefined":
            sys.exit("Could not fetch state from :"+filename)
        l = len(self.parameters[0])
        for i in range(1, len(self.parameters)):
            if len(self.parameters[i]) != l:
                sys.exit("Length of parameter set " +
                         str(i+1)+" mismatched previous one")

    def write(self, filename):
        statefile = open(filename, "w+")
        statefile.write("#State = "+self.state+"\n")
        statefile.write(self.comments+"\n")
        for i in range(len(self.parameters)):
            for j in range(len(self.parameters[i])):
                statefile.write('%e' % self.parameters[i][j]+" ")
            statefile.write(str(self.solutions[i])+" pending\n")
        statefile.close()


usage = "usage: %prog [options] statefile-in statefile-out"
parser = argparse.ArgumentParser(description=usage)

parser.add_argument("--eps", dest="eps", metavar="EPS", type=float,
                  help="tolerance for initialization", default=0.1)
(options, args) = parser.parse_known_args()


try:
    import cma
except:
    sys.exit(
        "cma module could not be imported, please make sure to cma.py in your PYTHONPATH.")

try:
    import numpy
except:
    sys.exit("Could not import numpy modules used by cma")


if len(args) != 2:
    sys.exit("two statefile required as parameters")

current_state = state()
current_state.read(args[0])
print("We are in State '", current_state.state, "' with parameters\n",
      current_state.parameters, "solutions: ", current_state.solutions)

if current_state.state == "Initialization":
    if len(current_state.parameters) != 1:
        sys.exit(
            "In Initialization step the state file should contain only one set (line)")
    if 'CSG_RUNTEST' in os.environ:
        opts = cma.CMAOptions()
        opts.set('seed', 1984)
        es = cma.CMAEvolutionStrategy(current_state.parameters[0], options.eps, opts)
    else:
        es = cma.CMAEvolutionStrategy(current_state.parameters[0], options.eps)
else:
    [es, X] = pickle.load(open("cma.internal_state.cur"))
    if not numpy.allclose(X, current_state.parameters):
        sys.exit("Parameterfile mismatches with internally saved parameters")
    es.tell(X, current_state.solutions)

new_state = state()
new_state.state = "Running"
new_state.parameters = es.ask()
new_state.solutions = [0 for i in range(len(new_state.parameters))]
new_state.comments = current_state.comments
print("We going to State '", new_state.state, "' with parameters\n",
      new_state.parameters, "solutions: ", new_state.solutions)
new_state.write(args[1])
# we need to pickle parameters as well as they are saved in a dict (string compare)
# and internal precission is float64
pickle.dump([es, new_state.parameters], open("cma.internal_state.new", 'wb'))
