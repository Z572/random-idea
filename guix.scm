(define-module (arximboldi packages schmutz)
  #:use-module (rnrs io ports)
  #:use-module (ice-9 popen)
  #:use-module (ice-9 rdelim)
  #:use-module (guix packages)
  #:use-module (guix download)
  #:use-module (guix gexp)
  #:use-module (guix git-download)
  #:use-module (guix build-system meson)
  #:use-module (guix licenses)
  #:use-module (guix build-system cmake)
  #:use-module (gnu packages guile)
  #:use-module (gnu packages pkg-config)
  #:use-module (gnu packages boost)
  #:use-module (gnu packages guile-xyz)
  #:use-module (gnu packages libffi)
  #:use-module (gnu packages version-control))

(use-modules (gnu packages llvm)
             (guix packages)
             (gnu packages ninja)
             (guix gexp)
             (guix build-system)
             (guix utils))

(define-public llvm-with-mlir
  (package
    (inherit llvm-15)
    (name "llvm-with-mlir")
    (outputs '("out"))
    (build-system
      (build-system-with-c-toolchain
       (package-build-system llvm-15)
       (list (list "clang-toolchain" clang-toolchain-15))))
    (arguments
     (substitute-keyword-arguments (package-arguments llvm-15)
       ((#:configure-flags flags #~'())
        #~(cons*
           "-DLLVM_TARGETS_TO_BUILD=host;RISCV"
           "-DLLVM_ENABLE_PROJECTS=mlir;clang"
           #$flags))))))

(define %source-dir (dirname (current-filename)))

(define-public guile-mlir
  (package
    (name "guile-mlir")
    (version "git")
    (source (local-file %source-dir
                        #:recursive? #t
                        #:select? (git-predicate %source-dir)))
    (build-system cmake-build-system)
    (arguments `(#:tests? #f))
    (native-inputs
     (list pkg-config))
    (inputs
     (list guile-3.0 llvm-with-mlir boost
           libffi
           schmutz))
    (home-page "https://github.com/arximboldi/schmutz")
    (synopsis "Bind C++ code to Scheme")
    (description "Bind C++ code to Scheme")
    (license boost1.0)))

guile-mlir
