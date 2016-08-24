(defun test () 1)

(defun look-for-config-file ()
  (with-open-file (config (merge-pathnames (user-homedir-pathname)
                                           (make-pathname :name "openmt"
                                                          :type "conf"))
                          :direction :io
                          :if-does-not-exist :create
                          :if-exists :overwrite)
    (let ((line (read-line config nil nil)))
      (if (null line)
        (format config "Place your device name here~%")
        (if (not (equalp line "Place your device name here"))
          line
          nil)))))
