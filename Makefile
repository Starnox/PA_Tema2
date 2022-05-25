
# tag-uri obligatorii (nume + comportament identic)
# build    => compileaza toata tema
#             (ATENTIE! E important - NU compilati in tag-urile de run. Sesizati?)
# run-p$ID => ruleaza problema cu ID-ul specificat (1, 2, 3, 4)
# clean    => sterge toate fisierele generate

# restul este la alegerea studentului
# TODO

LANG := cpp

CFLAGS = -Wall -Wextra -g --std=c++17

# nume necesar (build)
build:
	g++ $(CFLAGS) curatare.cpp -o curatare
	g++ $(CFLAGS) fortificatii.cpp -o fortificatii
	g++ $(CFLAGS) beamdrone.cpp -o beamdrone

run-p1:
	./curatare

run-p2:
	./fortificatii

run-p3:
	./beamdrone	

run-p4:
	

run-p5:

clean:
	rm -f curatare fortificatii beamdrone
