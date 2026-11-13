# Compilation : make   |  Execution : ./gestion_vols  |  Nettoyage : make clean
# (recettes ecrites apres ";" sur la meme ligne : aucune tabulation necessaire)
CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude
SRC      = $(wildcard src/*.cpp)
TARGET   = gestion_vols

all: $(TARGET)

$(TARGET): $(SRC) $(wildcard include/*.h) ; $(CXX) $(CXXFLAGS) -o $@ $(SRC)

clean: ; rm -f $(TARGET)
