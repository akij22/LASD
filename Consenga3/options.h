// File per la gestione delle opzioni di compilazione via command line

#pragma once

struct ProgramOptions {
    bool verbose;
    bool graph;
};

ProgramOptions options_default();
bool parse_cmd(int argc, char **argv, ProgramOptions *options);
