CREATE TABLE Distributor(Company_name VARCHAR(30),
                         Address VARCHAR(50) not null,
                         Marketing_budget NUMBER(19,4) not null,
                         Platforms VARCHAR(80) not null,
                         Box_Office_target NUMBER(19,4) not null,
                         PRIMARY KEY (Company_name));
                         
CREATE TABLE Film(Title VARCHAR(30),
                         Release_date DATE,
                         Film_distributor VARCHAR(30) not null,
                         Director VARCHAR(30) not null,
                         Script_writer VARCHAR(30) not null,
                         Film_budget NUMBER(19,4) not null,
                         FOREIGN KEY(Film_distributor) REFERENCES Distributor(Company_name),
                         PRIMARY KEY (Title, Release_date));

CREATE TABLE Production_Team(pPPS_No NUMBER(9,0),
                         pName VARCHAR(30) not null,
                         pAge VARCHAR(3) not null,
                         pGender VARCHAR(15) not null,
                         Role VARCHAR(30) not null,
                         pSalary NUMBER(19,4) not null,
                         pFilm_title VARCHAR(30) not null, 
                         pFilm_release DATE not null,
                         PRIMARY KEY (pPPS_No),
                         FOREIGN KEY(pFilm_title, pFilm_release) REFERENCES Film(Title, Release_date),
                         CONSTRAINT check_pPPS_No CHECK (pPPS_No < 1000000000));
                         
CREATE TABLE Department(Name VARCHAR(20),
                         Manager NUMBER(9,0) not null,
                         Responsibility VARCHAR(100) not null,
                         Dept_budget NUMBER(19,4) not null,
                         PRIMARY KEY (Name),
                         FOREIGN KEY(Manager) REFERENCES Production_Team(pPPS_No),
                         CONSTRAINT check_Manager CHECK (Manager < 1000000000));

CREATE TABLE Crew(Crew_PPS_No NUMBER(9,0),
                         Crew_name VARCHAR(30) not null,
                         Crew_age VARCHAR(3) not null,
                         Crew_gender VARCHAR(15) not null,
                         Job VARCHAR(30) not null,
                         Crew_contract VARCHAR(30) not null,
                         Crew_salary NUMBER(19,4) not null,
                         Department_name VARCHAR(20) not null,
                         Crew_film_title VARCHAR(30) not null,
                         Crew_film_release DATE not null,
                         PRIMARY KEY (Crew_PPS_NO),
                         FOREIGN KEY(Department_name) REFERENCES Department(Name),
                         FOREIGN KEY(Crew_film_title, Crew_film_release) REFERENCES Film(Title, Release_date),
                         CONSTRAINT check_Crew_PPS_No CHECK (Crew_PPS_No < 1000000000));

CREATE TABLE Equipment(Equip_ID INTEGER,
                         Model VARCHAR(30) not null,
                         Type VARCHAR(30) not null,
                         Status NUMBER(1) not null,
                         Crew_mem NUMBER(9,0) not null,
                         Cost INTEGER not null,
                         PRIMARY KEY (Equip_ID),
                         FOREIGN KEY(Crew_mem) REFERENCES Crew(Crew_PPS_No),
                         CONSTRAINT check_Crew CHECK (Crew_mem < 1000000000));
                         
CREATE TABLE Department_equipment(Equip_No INTEGER,
                         dName VARCHAR(20),
                         PRIMARY KEY (Equip_No,dName),
                         FOREIGN KEY(Equip_No) REFERENCES Equipment(Equip_ID),
                         FOREIGN KEY(dName) REFERENCES Department(Name));
                      
CREATE TABLE Film_cast(aPPS_No NUMBER(9,0),
                         aName VARCHAR(30) not null,
                         Part VARCHAR(30) not null,
                         aContract VARCHAR(30) not null,
                         aAge VARCHAR(3) not null,
                         aGender VARCHAR(15) not null,
                         aFilm_title VARCHAR(30) not null,
                         aFilm_release DATE not null,
                         aDistributor VARCHAR(30) not null,
                         PRIMARY KEY (aPPS_No),
                         FOREIGN KEY(aDistributor) REFERENCES Distributor(Company_name),
                         FOREIGN KEY(aFilm_title, aFilm_release) REFERENCES Film(Title, Release_date),
                         CONSTRAINT check_aPPS_No CHECK (aPPS_No < 1000000000));

INSERT INTO Distributor VALUES('20th Century Fox', '10201 W Pico Blvd, Los Angeles, CA 90064', 750000, 'Cinema Netflix TV boxset', 5000000);
                         
INSERT INTO Film VALUES('Brew Vervet', '13-DEC-15', '20th Century Fox', 'David Lynch', 'David Lynch', 1500000);

INSERT INTO Production_Team VALUES(987654321, 'Don Draper', '47', 'Male', 'Head Producer', 386000, 'Brew Vervet', '13-DEC-15');
INSERT INTO Production_Team VALUES(321321321, 'Joan Holloway', '39', 'Female', 'Hair/Makeup lead', 136000, 'Brew Vervet', '13-DEC-15');
INSERT INTO Production_Team VALUES(111444111, 'Ted Chaough', '41', 'Male', 'Art lead', 286000, 'Brew Vervet', '13-DEC-15');
INSERT INTO Production_Team VALUES(155444551, 'Roger Sterling', '57', 'Male', 'Sound Production lead', 319000, 'Brew Vervet', '13-DEC-15');

INSERT INTO Department VALUES('Art Department', 111444111, 'production of art assets',65000);
INSERT INTO Department VALUES('Camera and Lighting', 987654321, 'filming and lighting scenes',105000);
INSERT INTO Department VALUES('Stunt Team', 987654321, 'coordination and execution of stunts',32500);
INSERT INTO Department VALUES('Hair and Makeup', 321321321, 'design and apply hair and makeup for actors',24000);
INSERT INTO Department VALUES('Costume Department', 987654321, 'design and creation of costumes',65000);
INSERT INTO Department VALUES('Sound Production', 155444551, 'mixing, mastering, recording and editing the audio for the film',85000);
INSERT INTO Department VALUES('Inventory', 155444551, 'Maintaining and storing unused equipment',2500);

INSERT INTO Crew VALUES(55556123, 'Pete Campbell', '33', 'Male', 'Inventory Caretaker', '12 Months', 46000, 
'Inventory', 'Brew Vervet', '13-DEC-15');                         
INSERT INTO Crew VALUES(123456789, 'Peggy Olson', '28', 'Female', 'Graphic Designer', '6 Months', 36000, 
'Art Department', 'Brew Vervet', '13-DEC-15');
INSERT INTO Crew VALUES(123456788, 'Bob Benson', '64', 'Male', 'Lighting Technician', '12 Months', 46000, 
'Camera and Lighting', 'Brew Vervet', '13-DEC-15');
INSERT INTO Crew VALUES(222244445, 'Michael Ginsberg', '33', 'Male', 'Stuntman', '9 Months', 27500, 
'Stunt Team', 'Brew Vervet', '13-DEC-15');
INSERT INTO Crew VALUES(111333555, 'Salvatore Romano', '51', 'Male', 'Artist', '6 Months', 31000, 
'Art Department', 'Brew Vervet', '13-DEC-15');
INSERT INTO Crew VALUES(999999999, 'Paul Kinsey', '36', 'Male', 'Cinematographer', '12 Months', 69000, 
'Camera and Lighting', 'Brew Vervet', '13-DEC-15');
INSERT INTO Crew VALUES(236623666, 'Betty Draper', '38', 'Female', 'Hair/Makeup staff', '12 Months', 56000, 
'Hair and Makeup', 'Brew Vervet', '13-DEC-15');
INSERT INTO Crew VALUES(666666666, 'Harry Crane', '42', 'Male', 'Costume Designer', '6 Months', 25000, 
'Costume Department', 'Brew Vervet', '13-DEC-15');
INSERT INTO Crew VALUES(555555555, 'Stan Rizzo', '31', 'Male', 'Audio Technician', '9 Months', 43000, 
'Sound Production', 'Brew Vervet', '13-DEC-15');
 
INSERT INTO Equipment VALUES(123, 'Canon', 'Camera', 1, 999999999, 2400);
INSERT INTO Equipment VALUES(124, 'Cinelight', 'Light', 1, 123456789, 2300);
INSERT INTO Equipment VALUES(125, 'Action Factory', 'Stunt gear', 1, 222244445, 1100);
INSERT INTO Equipment VALUES(126, 'Behringer', 'Mixing board', 1, 555555555, 1700);

INSERT INTO Department_equipment VALUES(123, 'Camera and Lighting');
INSERT INTO Department_equipment VALUES(124, 'Camera and Lighting');
INSERT INTO Department_equipment VALUES(125, 'Stunt Team');
INSERT INTO Department_equipment VALUES(126, 'Sound Production');

INSERT INTO Film_cast VALUES('777747777', 'Kyle MacLachlan', 'Jeffrey Beaumont', '14 Months', '27', 'Male', 'Brew Vervet', '13-DEC-15', '20th Century Fox');
INSERT INTO Film_cast VALUES('667747766', 'Isabella Rossellini', 'Dorothy Vallens', '12 Months', '32', 'Female', 'Brew Vervet', '13-DEC-15', '20th Century Fox');
INSERT INTO Film_cast VALUES('577747775', 'Dennis Hopper', 'Frank Booth', '14 Months', '53', 'Male', 'Brew Vervet', '13-DEC-15', '20th Century Fox');
INSERT INTO Film_cast VALUES('734747437', 'Laura Dern', 'Sandy Williams', '12 Months', '26', 'Female', 'Brew Vervet', '13-DEC-15', '20th Century Fox');
INSERT INTO Film_cast VALUES('511181555', 'Dean Stockwell', 'Ben', '6 Months', '41', 'Male', 'Brew Vervet', '13-DEC-15', '20th Century Fox');

CREATE OR REPLACE TRIGGER New_budget
    AFTER UPDATE ON Department
    FOR EACH ROW
WHEN (NEW.Dept_budget != OLD.Dept_budget)
DECLARE
    adjusted_budget NUMBER(19,4);
BEGIN
    adjusted_budget := :NEW.Dept_budget - OLD.Dept_budget;
    UPDATE Film
        SET Film_budget = Film_budget + adjusted_budget;
END New_budget;
.
RUN;

CREATE OR REPLACE TRIGGER Crew_Removal
    BEFORE DELETE ON Crew
    FOR EACH ROW
DECLARE
    new_PPS VARCHAR(9);
    increased_budget NUMBER(19,4);
    dept VARCHAR(20);
BEGIN
    new_PPS := :OLD.Crew_PPS_No;
    UPDATE Equipment
    SET Crew_mem = '55556123'
    WHERE Crew_mem = new_PPS;
    increased_budget := :OLD.Crew_salary;
    dept := :OLD.Department_name;
    UPDATE Department
    SET Dept_budget = Dept_budget + increased_budget
    WHERE Name = dept;
END Crew_Removal;
.
RUN;

CREATE OR REPLACE VIEW Department_info
    AS SELECT Crew_name, Name, Responsibility FROM Crew, Department
        WHERE Crew.Department_name = Department.Name;

CREATE OR REPLACE VIEW Equipment_info
    AS SELECT Equip_ID, Model, Type, Status, Cost, Crew_name FROM Equipment, Crew
        WHERE Equipment.Crew_mem = Crew.Crew_PPS_No;

DECLARE
  producer_pps NUMBER(9,0) := 987654321;
  base_bonus NUMBER := 5000;
  christmas_bonus NUMBER;
BEGIN
  SELECT pSalary * 0.10 + base_bonus INTO christmas_bonus FROM Production_Team 
    WHERE pPPS_No = producer_pps;
  dbms_output.put_line(christmas_bonus);
  dbms_output.put_line('Christmas bonus for ' || producer_pps || ' is ' || christmas_bonus);
END;
.
RUN;
--Security!
    --CREATE USER GeneralEmployee;
    --CREATE USER PeteCampbell;
    --CREATE USER BertCooper;
    
    --CREATE ROLE Inventory_Man IDENTIFIED by diablo;
    --GRANT SELECT ON Equipment_info TO Inventory_Man;
    --GRANT UPDATE Status ON Equipment TO Inventory_MAN;
    --GRANT CREATE TABLE TO Inventory_Man;
    --REVOKE CREATE TABLE TO Inventory_Man;
    
    --CREATE ROLE Payroll_Records IDENTIFIED by drseus;
    --GRANT UPDATE,SELECT,INSERT ON Crew TO Payroll_Records;
    --GRANT UPDATE,SELECT,INSERT ON Production_Team TO Payroll_Records;
    --GRANT UPDATE,SELECT,INSERT ON Film_cast TO Payroll_Records;
    --REVOKE CREATE TABLE TO Inventory_Man;
    
    --GRANT Inventory_Man TO PeteCampbell;
    --GRANT SELECT ON Department_info TO GeneralEmployee;
    --GRANT Payroll_Records TO BertCooper;


