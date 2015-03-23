all:
	$(MAKE) -C lib/
	$(MAKE) -C cat/
	$(MAKE) -C revwords/
	$(MAKE) -C filter/

clean:
	$(MAKE) -C lib/ clean
	$(MAKE) -C cat/ clean
	$(MAKE) -C revwords/ clean
	$(MAKE) -C filter/ clean

