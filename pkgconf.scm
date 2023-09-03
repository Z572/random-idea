(define-module (pkgconf)
  #:use-module (oop goops)
  #:use-module ((rnrs base) #:select (assert))
  #:use-module ((system foreign) #:prefix ffi:)
  #:export (pc:client-new
            pc:package-find
            .sysroot))

(eval-when (expand load eval)
  (load-extension "libguile_pkgconf" "init_pkgconf"))

(define-class <pc:client> (<%pc:client>)
  (sysroot #:allocation #:virtual
           #:slot-ref %client-sysroot-dir
           #:slot-set! %client-set-sysroot-dir
           #:accessor .sysroot))
(%set-pc:client-type)

(define* (pc:client-new #:optional (proc #f) (personality #f) (build-list #t))
  (when proc
    (assert (procedure? proc)))
  (when personality
    (assert (string? personality)))

  (define (pc:client-new:error-handle msg client data)
    (if (proc (ffi:pointer->string msg)
              (%make-foreign-object <pc:client> client))
        1
        0))
  (%client-new (if proc
                   (ffi:procedure->pointer
                    ffi:int pc:client-new:error-handle
                    (list '* '* '*))
                   ffi:%null-pointer)
               personality
               build-list))

(define (pc:package-find client name)
  (assert (string? name))
  (%pc:package-find client name))
