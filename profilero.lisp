(defvar *program-name* "Primos")
(defvar *program-control-string* "./~a --qt-threads=~a --output=~a")

(defun execute-program (&optional n)
  "Executes the program defined at top level, while checking if it exists.
It will always execute said program while passing ``n'' as an argument and 0 in
order to silence its output"
  (if (uiop:file-exists-p *program-name*)
      (read-from-string
       (uiop:run-program (format nil *program-control-string* *program-name* n 0)
			 :output :string))
    (error "You should compile Main.cpp")))

(defparameter *total-runs* 50)

(defun get-average (runs &optional (n 1))
  "Runs the program x times (runs) and returns the average runtime (sum/runs).
``n'' refers to the number of threads in which our program must execute on,
defaults to 1."
  (loop for i
	from 0 below runs
	sum (execute-program n) into result
	finally (return (/ result runs))))

(defun get-speedup (serial-time threaded-time)
  "Returns the speedup (``serial-time''/``threaded-time'') of a process/job."
  (/ serial-time threaded-time))

(defparameter *thread-count* 10)

(defun get-row (n &key serial)
  "Returns a single row of our table.
Having the number of threads to execute the main program being ``n'',
this procedure will return a list with the following structure:

(number-of-threads (average-time . speedup))

The optional keyword argument, ``serial'' tells us that the serial time was
supplied, otherwise the average time will be divided by itself in order to
obtain speedup info"
  (let* ((time (get-average *total-runs* n))
	 (speedup (get-speedup (or serial time) time)))
    (list n time speedup)))

(defun produce-table ()
  "Produces the main table to be read by org."
  (let* ((serial-row (get-row 1))
	 (serial-time (second serial-row))
	 (parallel-table (loop for n from 2 to *thread-count*
			       collect (get-row n :serial serial-time ))))
    (cons serial-row parallel-table)))

(produce-table)
