:- load_foreign_files(['yap2postgres'],[],init_my_predicates).
:- consult(sql_compiler).

db_open(Host,Dbname,User,Passwd,ConnName) :-
  db_connect(Host,Dbname,User,Passwd,ConnHandler),
  set_value(ConnName,ConnHandler).

db_close(ConnName) :-
  get_value(ConnName,ConnHandler),
  db_disconnect(ConnHandler).