all:
	swig -python -c++ -py3 -o _stepik_alignment_module.cc stepik_alignment.i
	python setup.py build_ext --inplace
