let
    pkgs = import <nixpkgs> { };
    mkDerivation = import ./build/autotools.nix pkgs;
in
mkDerivation {
    name = "GenerateEncoder";
}