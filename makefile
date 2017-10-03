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

# Calls 'clean' for all makefiles in the project
deep-clean:
	$(info Preforming Deep Clean....)
	@cd ai && $(MAKE) clean
	@cd core/im && $(MAKE) clean

clean-ai:
	@cd ai && $(MAKE) clean

clean-im:
	@cd core/im && $(MAKE) clean