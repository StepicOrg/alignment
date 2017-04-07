rm *.so
make
python setup.py install
python test.py
python dictation_test.py all_tests