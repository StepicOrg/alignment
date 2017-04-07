all:
	swig -python -c++ -o _stepik_alignment_module.cc stepik_alignment.i
	python setup.py build_ext --inplace
