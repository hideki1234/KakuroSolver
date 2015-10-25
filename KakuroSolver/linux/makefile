MKDIR = mkdir -p
RMF = rm -f
RMRF = rm -rf
CPP = g++
CPPFLAGS = -c -std=c++11

ifdef DEBUG
	BUILD_DIR=Debug
	CPPFLAGS += -g -O0
else
	BUILD_DIR=Release
	CPPFLAGS += -O3
endif

all: solver

$(BUILD_DIR):
	@$(MKDIR) $(BUILD_DIR)

#
# solver build
#
SOLVER_OBJS = innerbrd.o kkrsolv_t.o slvfactr.o solvimpl.o version.o
SOLVER_DIR = ../KakuroSolver_t
SOLVER_BLD_OBJS = $(addprefix $(BUILD_DIR)/, $(SOLVER_OBJS))

$(BUILD_DIR)/%.o : $(SOLVER_DIR)/%.cpp
	$(CPP) -I$(SOLVER_DIR) $(CPPFLAGS) -o $@ $<

$(BUILD_DIR)/libkkrs.a : $(SOLVER_BLD_OBJS)
	@$(RMF) $@
	ar cq $@ $(SOLVER_BLD_OBJS)

solver: $(BUILD_DIR) $(BUILD_DIR)/libkkrs.a

#
# solver tester
#
SOLTST_OBJS = testSlv1.o testSlv2CellSet.o testSlv3Cell.o testSlv4brd.o testSlv5simple.o testSlv6Cmplx.o testSlv7LrgUnq.o testSlvMain.o
SOLTST_DIR = ../testSolver
SOLTST_BLD_OBJS = $(addprefix $(BUILD_DIR)/, $(SOLTST_OBJS))

$(BUILD_DIR)/%.o : $(SOLTST_DIR)/%.cpp
	$(CPP) -I$(SOLTST_DIR) $(CPPFLAGS) -o $@ $<

$(BUILD_DIR)/testSolver : solver $(SOLTST_BLD_OBJS)
	$(CPP) -o $@ $(SOLTST_BLD_OBJS) $(BUILD_DIR)/libkkrs.a

solvertest: $(BUILD_DIR)/testSolver

#
# clean
#
clean:
	@$(RMRF) $(BUILD_DIR)
