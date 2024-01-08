from __future__ import division
import numpy as np
np.set_printoptions(precision=3)

import sys as _sys
#import freecad
_path_to_freecad_libdir = "/mnt/work2/artur/freecad/squashfs-root/usr/lib"
_sys.path.append(_path_to_freecad_libdir) # this is the default version
import FreeCAD as App

def mat_to_np(mat):
    elements = mat.A
    return np.array([[elements[j * 4 + i] for i in range(4)] for j in range(4)])

def np_to_mat(np_mat):
    App.Matrix(*np_mat.flatten())

def vec_to_np(vec):
    return np.array(list(vec))

#App.loadFile('/mnt/work2/artur/3D-model/rattleback_left.fcstd')
App.loadFile('/mnt/work2/artur/3D-model/RotationBox1.fcstd')
App.loadFile('/mnt/work2/artur/3D-model/solids/rattleback-solid-v001.FCStd')
document = App.getDocument(list(App.listDocuments().keys())[0])
obj = document.Objects[0]
inert = mat_to_np(obj.Shape.MatrixOfInertia) 
mass = obj.Shape.Volume 
cog = vec_to_np(obj.Shape.CenterOfMass)
