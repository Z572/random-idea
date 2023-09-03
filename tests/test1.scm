(define-module (tests test1)
  #:use-module (pkgconf)
  #:use-module (oop goops)
  #:use-module (oop goops describe)
  #:use-module (srfi srfi-64))

(test-group "pkgconf"
  (test-assert "pc:client-new : no arg"
    (pc:client-new))
  (test-assert "pc:client-new : 1 arg"
    (pc:client-new (lambda (msg cli) (pk msg cli) #t)))
  (test-assert "pc:client-new : 2 arg"
    (pc:client-new (lambda (msg cli) (pk msg cli)) "riscv64-gnu-linux"))
  (test-assert "pc:package-find"
    (pc:package-find (pc:client-new) "guile-3.0"))
  (test-equal "pc:package-find : fail"
    #f
    (pc:package-find (pc:client-new) "kkjjb"))
  (test-equal "<pc:client> : sysroot #f"
    #f (.sysroot (pc:client-new)))
  (test-equal "<pc:client> : sysroot set!"
    "/tmp" (let ((o (pc:client-new)))
             (set! (.sysroot o) "/tmp")
             (.sysroot o)))
  (test-assert "<pc:client> : describe"
    (describe (pc:client-new))))
