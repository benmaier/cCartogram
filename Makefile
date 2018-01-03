clean_all:
	make pyclean
	make matclean

pyclean:
	-rm -f *.so
	-rm -rf *.egg-info*
	-rm -rf ./tmp/
	-rm -rf ./build/

matclean:
	-rm -rf ./matlabbuild/
