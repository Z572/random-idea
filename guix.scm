(use-modules
 (gnu packages tree-sitter)
 (guix utils) (guix packages)
 ((guix licenses) #:prefix license:)
 (gnu packages xorg)
 (guix download)
 (guix git-download)
 (gnu packages gettext)
 (guix gexp)
 (gnu packages gl)
 (gnu packages xdisorg)
 (guix build-system gnu)
 (gnu packages bash)
 (gnu packages)
 (gnu packages autotools)
 (gnu packages guile)
 (gnu packages gtk)
 (gnu packages guile-xyz)
 (gnu packages ibus)
 (gnu packages pkg-config)
 (gnu packages texinfo)
 (gnu packages wm)
 (gnu packages freedesktop))

(define %srcdir
  (dirname (current-filename)))

(define-public guile-pkgconf
  (package
    (name "guile-pkgconf")
    (version "0.0.1")
    (source (local-file "." "guile-pkgconf"
                        #:recursive? #t
                        #:select? (git-predicate %srcdir)))
    (build-system gnu-build-system)
    (arguments
     (list #:make-flags #~(list "GUILE_AUTO_COMPILE=0")
           #:phases
           #~(modify-phases %standard-phases
               (add-after 'build 'load-extension
                 (lambda* (#:key outputs #:allow-other-keys)
                   (substitute*
                       (find-files "." ".*\\.scm")
                     (("\\(load-extension \"libguile_pkgconf\" *\"(.*)\"\\)" _ o)
                      (string-append
                       (object->string
                        `(or (false-if-exception (load-extension "libguile_pkgconf" ,o))
                             (load-extension
                              ,(string-append
                                #$output
                                "/lib/libguile_pkgconf.so")
                              ,o)))))))))))
    (native-inputs
     (list autoconf automake
           libtool
           pkg-config
           guile-3.0-latest))
    (inputs (list guile-3.0-latest pkgconf))
    (synopsis "")
    (description "")
    (home-page "")
    (license license:gpl3+)))
guile-pkgconf
