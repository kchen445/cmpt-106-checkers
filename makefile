###################
# MASTER MAKEFILE #
###################


# Archives #

imlib.a:
	@cd core/im && $(MAKE) lib

ailib.a:
	@cd ai && $(MAKE) lib


# Executables #

# builds ailib.a from local makefile
# does not build imlib.a though, so we build it here
aiexe: imlib.a
	@cd ai && $(MAKE) exe

imexe:
	@cd core/im && $(MAKE) exe


# Cleaning #

# Removes all object (.o), archive (.a), and executable files from the project.
purge:
	$(info Preforming binary file purge...)
	$(info Removing object files...)
	@rm -f $(shell find . -name '*.o')
	$(info Removing archive files...)
	@rm -f $(shell find . -name '*.a')
	$(info Removing executables...)
	@rm -f bin/*
	$(info Done)

clean-ai:
	@cd ai && $(MAKE) clean

clean-im:
	@cd core/im && $(MAKE) clean