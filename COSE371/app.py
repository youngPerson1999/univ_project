import psycopg2
from flask import Flask, render_template, request, redirect, url_for

app = Flask(__name__)
connect = psycopg2.connect("dbname=tutorial user=postgres password=0000")
cur = connect.cursor()  # create cursor


@app.route('/')
def main():
    return render_template("main.html")

@app.route('/return', methods=['post'])
def re_turn():
    return render_template("main.html")

@app.route('/register', methods=['post'])
def register():
    id = request.form["id"]
    password = request.form["password"]
    send = request.form["send"]

    if send=="sign up":
        return render_template("signup.html")

    if send=="login":
        cur.execute("SELECT id FROM users WHERE id = %s AND password = %s", (id, password))
        result = cur.fetchone()
        if result:
            return redirect(url_for('info', id=id))
        else:
            return render_template("login_fail.html", is_admin=False)
            
    return id + " " + password + " " + send

@app.route('/sign-up', methods=['post'])
def signUp():
    id = request.form["id"]
    password = request.form["password"]
    role = request.form["role"]

    try:
        # INSERT 문 생성
        insert_query = "INSERT INTO users (id, password) VALUES (%s, %s);"
        insert_values = (id, password)

        # users 테이블에 사용자 정보 삽입
        cur.execute(insert_query, insert_values)

        # account 테이블에 사용자 계정 정보 삽입
        account_insert_query = "INSERT INTO account (id, credit, rating, role) VALUES (%s, %s, %s, %s);"
        account_insert_values = (id, 10000, 'welcome', role)
        cur.execute(account_insert_query, account_insert_values)

        # 커밋하여 변경 사항을 데이터베이스에 반영
        connect.commit()

        return redirect(url_for('main'))
    except psycopg2.IntegrityError:
        connect.rollback()
        return render_template("id_collision.html")

@app.route('/info', methods=['get'])
def info():
    id = request.args.get('id')
    is_admin = False
    if id=="admin":
        is_admin = True
    # popular subject, lecture, tutor 출력해주기
    cur.execute("SELECT s.subject_name FROM enrollment e \
        JOIN subject s ON e.code = s.code \
        GROUP BY s.code, s.subject_name \
        ORDER BY COUNT(*) DESC LIMIT 1;")
    subject = cur.fetchone()
    cur.execute("SELECT name FROM lecture \
        ORDER BY ( \
        SELECT COUNT(*) \
        FROM enrollment \
        WHERE enrollment.code = lecture.code \
        AND enrollment.lecture_name = lecture.name \
        AND enrollment.lecture_price = lecture.price \
        AND enrollment.tutor = lecture.tutor \
        ) DESC LIMIT 1;")
    lecture = cur.fetchone()
    cur.execute("SELECT tutor, COUNT(tutor) AS tutor_count FROM enrollment GROUP BY tutor ORDER BY tutor_count DESC LIMIT 1;")
    tutor = cur.fetchone()
    #  유저 정보
    cur.execute("SELECT id, credit, rating, role FROM account WHERE id = %s;", (id,))
    u_info = cur.fetchone()
    is_tutor = False
    if u_info[3]=="tutor":
        is_tutor = True
    # 등록가능한 강의 테이블
    cur.execute("select * from lecture except select code, lecture_name as name, lecture_price as price, tutor from enrollment where tutee=%s;",(id,))
    possibleLectures = cur.fetchall()
    return render_template("info/index.html", \
        is_admin=is_admin, \
        populars=[subject[0], \
        lecture[0], tutor[0]], \
        u_info=u_info, \
        is_tutor=is_tutor, \
        lectures=possibleLectures)

@app.route('/logout', methods=['get'])
def logout():
    return redirect(url_for('main'))

@app.route('/mylecture', methods=['get'])
def mylecture():
    id=request.args.get('id')
    cur.execute("select role from account where id = %s;",(id,))
    is_tutor=True if (cur.fetchone())[0]=="tutor" else False

    if is_tutor:
        cur.execute("SELECT s.subject_name, l.name, e.tutee, l.price \
            FROM lecture l \
            JOIN subject s ON l.code = s.code \
            LEFT JOIN enrollment e ON l.code = e.code AND l.tutor = e.tutor \
            WHERE l.tutor = %s;",(id,))
    tutorLectures = cur.fetchall() if is_tutor else []
    
    cur.execute("SELECT s.subject_name, e.lecture_name, e.tutor, e.lecture_price \
        FROM subject s JOIN enrollment e ON s.code = e.code \
        WHERE e.tutee = %s;", (id,))
    mylectures = cur.fetchall()
    
    return render_template('info/mylecture.html', tutorLectures=tutorLectures, is_tutor=is_tutor, mylectures=mylectures, id=id)

@app.route('/add', methods=['get'])
def addForm():
    id=request.args.get('id')
    cur.execute("select code, subject_name from subject")
    subjectList = cur.fetchall()
    
    return render_template('info/add.html', subjects=subjectList, tutor_id=id)

@app.route('/add', methods=['post'])
def add():
    code = request.form.get('code')
    name = request.form.get('name')
    price = request.form.get('price')
    tutor = request.form.get('id')

    if len(code) > 2:
        return render_template('info/fail.html', fail="Add")
    
    try:
        price = int(price)  # Convert price to integer
    except ValueError:
        return render_template('info/fail.html', fail="Add")
    
    sql = "INSERT INTO lecture (code, name, price, tutor) VALUES (%s, %s, %s, %s);"
    values = (code, name, price, tutor)
    
    try:
        cur.execute(sql, values)
        connect.commit()
        return redirect(url_for('info', id=tutor))
    except psycopg2.IntegrityError:
        connect.rollback()
        return render_template('info/fail.html', fail="Add")

@app.route('/pay', methods=['get'])
def payForm():
    id=request.args.get('id')
    code=request.args.get('code')
    name=request.args.get('name')
    price=request.args.get('price')
    tutor=request.args.get('tutor')
    lecture = [code, name, price, tutor]
    
    cur.execute("select credit, rating from account where id = %s;", (id,))
    result = cur.fetchone()
    cur.execute("select condition, discount from rating_info where rating = %s;",(result[1],))
    rating_info=cur.fetchone()
    
    tutee = [id, result[0], result[1]]
    pay = [rating_info[1]*int(price), int(price)- rating_info[1]*int(price)]
    
    return render_template('info/pay_lecture.html', tutee=tutee, lecture=lecture, pay=pay)

@app.route('/pay', methods=['post'])
def pay():
    tutee=request.form.get('tutee')
    tutor=request.form.get('tutor')
    price=request.form.get('price')
    name=request.form.get('name')
    code=request.form.get('code')
    total_price=request.form.get('t_price')

    cur.execute("select rating, condition from rating_info order by condition desc;")
    rating_info=cur.fetchall()
    cur.execute("select credit from account where id = %s;", (tutee,))
    tutee_info = cur.fetchone()
    
    updated_credit = int(tutee_info[0]) - float(total_price)
    updated_rating = ""
    
    for rating, condition in rating_info:
        if updated_credit >= condition:
            updated_rating = rating
    try:
        sql = "INSERT INTO enrollment (tutee, tutor, code, lecture_name, lecture_price) \
            VALUES(%s, %s, %s, %s, %s);"
        values = (tutee, tutor, code, name, price)
        cur.execute(sql, values)

        cur.execute("update account set credit = %s, rating = %s where id=%s;",(updated_credit,updated_rating, tutee))
        connect.commit()
    except psycopg2.IntegrityError:
        connect.rollback()
        return render_template('info/fail.html', fail="pay")

    return redirect(url_for('info', id=tutee))

@app.route('/edit', methods=['get'])
def editForm():
    id=request.args.get('id')
    return render_template('edit.html', id=id)

@app.route('/edit', methods=['post'])
def edit():
    id=request.form.get('id')
    pw=request.form.get('cur_pw')
    new_pw=request.form.get('new_pw')
    
    cur.execute("select password from users where id=%s;",(id,))
    cur_pw = (cur.fetchone())[0]

    if pw != cur_pw:
        return render_template('info/fail.html', fail="Because pw not equal, edit")

    try:
        cur.execute("update users set password=%s where id=%s;",(new_pw, id))
        connect.commit()
    except psycopg2.IntegrityError:
        connect.rollback()
        return render_template('info/fail.html', fail="edit") 
      
    return redirect(url_for('info', id=id))

@app.route('/drop', methods=['get'])
def dropForm():
    id=request.args.get('id')
    subject=request.args.get('subject')
    name=request.args.get('name')
    price=request.args.get('price')
    tutor=request.args.get('tutor')

    refund = int(float(price)*0.9)
    info = [subject, name, price, tutor, refund]
    
    cur.execute("select credit from account where id=%s;",(id,))
    credit = (cur.fetchone())[0] + refund
    return render_template('info/drop_lecture.html', lecture=info, id=id, credit=credit)

@app.route('/drop', methods=['post'])
def drop():
    tutee=request.form.get('tutee')
    id = tutee
    tutor=request.form.get('tutor')
    name=request.form.get('name')
    refund=int(request.form.get('refund'))

    cur.execute("select rating, condition from rating_info order by condition desc;")
    rating_info=cur.fetchall()
    cur.execute("select credit from account where id = %s;", (id,))
    tutee_info = cur.fetchone()

    updated_credit = int(tutee_info[0]) + float(refund)
    updated_rating = ""
    for rating, condition in rating_info:
        if updated_credit >= condition:
            updated_rating = rating
    try:
        cur.execute("delete from enrollment where tutee=%s and tutor=%s and lecture_name=%s;", (tutee, tutor, name))
        cur.execute("update account set credit = %s, rating = %s where id=%s;",(updated_credit,updated_rating, tutee))
        connect.commit()
    except psycopg2.IntegrityError:
        connect.rollback()
        return render_template('info/fail.html', fail="drop")

    return redirect(url_for('info', id=id))

@app.route('/admin_func', methods=['get'])
def adminFunc():
    send=request.args.get('send')

    if send=="users info":
        cur.execute("SELECT u.id, a.role, a.credit, a.rating FROM users u JOIN account a ON u.id = a.id;")
        users = cur.fetchall()
        return render_template('print_table.html', users=users, id="users")   
    if send=="trades info":
        cur.execute("select tutee, tutor, code, lecture_name, lecture_price from enrollment")
        trades = cur.fetchall()
        return render_template('print_table.html', trades=trades, id="trades") 
    return render_template('info/fail.html', fail="load data")
if __name__ == '__main__':
    app.run()