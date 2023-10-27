project('glucose', 'cpp', version : '4.2.1',
  default_options : ['cpp_std=c++11'])

# -------------------
# BUILD instructions:
#
#   meson setup builddir --buildtype release --strip
#
#   meson compile -C builddir
#
#   meson install -C builddir
#
# -------------------

src = [
  'capi/cglucose.cc',
  'core/lcm.cc',
  'core/Solver.cc',
  'simp/SimpSolver.cc',
  'utils/Options.cc',
  'utils/System.cc',
]
src_parallel = [
  'parallel/ClausesBuffer.cc',
  'parallel/MultiSolvers.cc',
  'parallel/ParallelSolver.cc',
  'parallel/SharedCompanion.cc',
  'parallel/SolverCompanion.cc',
  'parallel/SolverConfiguration.cc',
]

zdep = dependency('zlib')
thread_dep = dependency('threads')

add_global_arguments('-DINCREMENTAL', language : 'cpp')

lib = library('glucose', src,
  install : true, dependencies : [zdep, thread_dep])
libp = library('glucosep', src, src_parallel,
  install : true, dependencies : [zdep, thread_dep])

exe_simp = executable('glucose', 'simp/Main.cc',
  install : true, link_with : lib, dependencies : zdep)
exe_parallel = executable('glucose-syrup', 'parallel/Main.cc',
  install : true, link_with : libp, dependencies : zdep)

install_headers(
  'capi/cglucose.h',
  'core/SolverTypes.h',
  'core/BoundedQueue.h',
  'core/Constants.h',
  'core/Dimacs.h',
  'core/Solver.h',
  'core/SolverStats.h',
  'mtl/Alg.h',
  'mtl/Alloc.h',
  'mtl/Clone.h',
  'mtl/Heap.h',
  'mtl/IntTypes.h',
  'mtl/Map.h',
  'mtl/Queue.h',
  'mtl/Sort.h',
  'mtl/Vec.h',
  'mtl/VecThreads.h',
  'mtl/XAlloc.h',
  'parallel/SolverConfiguration.h',
  'parallel/ClausesBuffer.h',
  'parallel/MultiSolvers.h',
  'parallel/ParallelSolver.h',
  'parallel/SharedCompanion.h',
  'parallel/SolverCompanion.h',
  'simp/SimpSolver.h',
  'utils/ParseUtils.h',
  'utils/System.h',
  'utils/Options.h',
  preserve_path : true,
  subdir : 'glucose',
)
