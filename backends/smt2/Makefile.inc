
OBJS += backends/smt2/smt2.o

ifneq ($(CONFIG),mxe)
ifneq ($(CONFIG),emcc)

# MSYS targets support yosys-smtbmc, but require a launcher script
ifeq ($(CONFIG),$(filter $(CONFIG),msys2 msys2-64))
TARGETS += $(PROGRAM_PREFIX)yosys-smtbmc.exe $(PROGRAM_PREFIX)yosys-smtbmc-script.py
TARGETS += $(PROGRAM_PREFIX)yosys-witness.exe $(PROGRAM_PREFIX)yosys-witness-script.py
# Needed to find the Python interpreter for yosys-smtbmc scripts.
# Override if necessary, it is only used for msys2 targets.
PYTHON := $(shell cygpath -w -m $(PREFIX)/bin/python3)

$(PROGRAM_PREFIX)yosys-smtbmc-script.py: backends/smt2/smtbmc.py
	$(P) sed -e 's|##yosys-sys-path##|sys.path += [os.path.dirname(os.path.realpath(__file__)) + p for p in ["/share/python3", "/../share/$(PROGRAM_PREFIX)yosys/python3"]]|;' \
		-e "s|#!/usr/bin/env python3|#!$(PYTHON)|" < $< > $@

$(PROGRAM_PREFIX)yosys-witness-script.py: backends/smt2/witness.py
	$(P) sed -e 's|##yosys-sys-path##|sys.path += [os.path.dirname(os.path.realpath(__file__)) + p for p in ["/share/python3", "/../share/$(PROGRAM_PREFIX)yosys/python3"]]|;' \
		-e "s|#!/usr/bin/env python3|#!$(PYTHON)|" < $< > $@

$(PROGRAM_PREFIX)yosys-smtbmc.exe: misc/launcher.c $(PROGRAM_PREFIX)yosys-smtbmc-script.py
	$(P) $(CXX) -DGUI=0 -O -s -o $@ $<

$(PROGRAM_PREFIX)yosys-witness.exe: misc/launcher.c $(PROGRAM_PREFIX)yosys-witness-script.py
	$(P) $(CXX) -DGUI=0 -O -s -o $@ $<
# Other targets
else
TARGETS += $(PROGRAM_PREFIX)yosys-smtbmc $(PROGRAM_PREFIX)yosys-witness

$(PROGRAM_PREFIX)yosys-smtbmc: backends/smt2/smtbmc.py
	$(P) sed 's|##yosys-sys-path##|sys.path += [os.path.dirname(os.path.realpath(__file__)) + p for p in ["/share/python3", "/../share/$(PROGRAM_PREFIX)yosys/python3"]]|;' < $< > $@.new
	$(Q) chmod +x $@.new
	$(Q) mv $@.new $@

$(PROGRAM_PREFIX)yosys-witness: backends/smt2/witness.py
	$(P) sed 's|##yosys-sys-path##|sys.path += [os.path.dirname(os.path.realpath(__file__)) + p for p in ["/share/python3", "/../share/$(PROGRAM_PREFIX)yosys/python3"]]|;' < $< > $@.new
	$(Q) chmod +x $@.new
	$(Q) mv $@.new $@
endif

$(eval $(call add_share_file,share/python3,backends/smt2/smtio.py))
$(eval $(call add_share_file,share/python3,backends/smt2/ywio.py))
endif
endif
