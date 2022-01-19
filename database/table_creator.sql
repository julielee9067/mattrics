CREATE TABLE mattrics.Therapist(
    _id int NOT NULL AUTO_INCREMENT,
    _createdDate timestamp NULL DEFAULT CURRENT_TIMESTAMP,
    _updatedDate timestamp NULL DEFAULT CURRENT_TIMESTAMP,
    _owner varchar(50) DEFAULT NULL,
    firstName varchar(30) NOT NULL,
    lastName varchar(30) NOT NULL,
    email varchar(30) NOT NULL,
    password varchar(30) NOT NULL,
    PRIMARY KEY (_id)
);

CREATE TABLE mattrics.Patient(
    _id int NOT NULL AUTO_INCREMENT,
    _createdDate timestamp NULL DEFAULT CURRENT_TIMESTAMP,
    _updatedDate timestamp NULL DEFAULT CURRENT_TIMESTAMP,
    _owner varchar(50) DEFAULT NULL,
    therapistId int NOT NULL,
    firstName varchar(30) NOT NULL,
    lastName varchar(30) NOT NULL,
    email varchar(30) NOT NULL,
    gmfcsLevel int NOT NULL,
    dateOfBirth DATE NOT NULL,
    PRIMARY KEY (_id),
    FOREIGN KEY (therapistId) REFERENCES mattrics.Therapist(_id)
);

CREATE TABLE mattrics.Rental(
    _id int NOT NULL AUTO_INCREMENT,
    _createdDate timestamp NULL DEFAULT CURRENT_TIMESTAMP,
    _updatedDate timestamp NULL DEFAULT CURRENT_TIMESTAMP,
    _owner varchar(50) DEFAULT NULL,
    patientId int NOT NULL,
    startDate DATE NOT NULL,
    endDate DATE NOT NULL,
    returnedDate DATE NOT NULL,
    PRIMARY KEY (_id),
    FOREIGN KEY (patientId) REFERENCES mattrics.Patient(_id)
);

CREATE TABLE mattrics.Pressure(
    _id int NOT NULL AUTO_INCREMENT,
    _createdDate timestamp NULL DEFAULT CURRENT_TIMESTAMP,
    _updatedDate timestamp NULL DEFAULT CURRENT_TIMESTAMP,
    _owner varchar(50) DEFAULT NULL,
    path varchar(200) NOT NULL,
    PRIMARY KEY (_id)
);

CREATE TABLE mattrics.Movement(
    _id int NOT NULL AUTO_INCREMENT,
    _createdDate timestamp NULL DEFAULT CURRENT_TIMESTAMP,
    _updatedDate timestamp NULL DEFAULT CURRENT_TIMESTAMP,
    _owner varchar(50) DEFAULT NULL,
    path varchar(200) NOT NULL,
    PRIMARY KEY (_id)
);

CREATE TABLE mattrics.Breathing(
    _id int NOT NULL AUTO_INCREMENT,
    _createdDate timestamp NULL DEFAULT CURRENT_TIMESTAMP,
    _updatedDate timestamp NULL DEFAULT CURRENT_TIMESTAMP,
    _owner varchar(50) DEFAULT NULL,
    AHI int,
    highestRate int,
    averageRate int,
    lowRatePercentage int,
    midRatePercentage int,
    highRatePercentage int,
    PRIMARY KEY (_id)
);

CREATE TABLE mattrics.DailyData(
    _id int NOT NULL AUTO_INCREMENT,
    _createdDate timestamp NULL DEFAULT CURRENT_TIMESTAMP,
    _updatedDate timestamp NULL DEFAULT CURRENT_TIMESTAMP,
    _owner varchar(50) DEFAULT NULL,
    patientId int NOT NULL,
    pressureId int NOT NULL,
    movementId int NOT NULL,
    breathingId int NOT NULL,
    timeToFallAsleep int,
    sleepAwakening int,
    PRIMARY KEY (_id),
    FOREIGN KEY (patientId) REFERENCES mattrics.Patient(_id),
    FOREIGN KEY (pressureId) REFERENCES mattrics.Pressure(_id),
    FOREIGN KEY (movementId) REFERENCES mattrics.Movement(_id),
    FOREIGN KEY (breathingId) REFERENCES mattrics.Breathing(_id)
);

CREATE TABLE mattrics.WeeklyData(
    _id int NOT NULL AUTO_INCREMENT,
    _createdDate timestamp NULL DEFAULT CURRENT_TIMESTAMP,
    _updatedDate timestamp NULL DEFAULT CURRENT_TIMESTAMP,
    _owner varchar(50) DEFAULT NULL,
    patientId int NOT NULL,
    pressureId int NOT NULL,
    movementId int NOT NULL,
    breathingId int NOT NULL,
    PRIMARY KEY (_id),
    FOREIGN KEY (patientId) REFERENCES mattrics.Patient(_id),
    FOREIGN KEY (pressureId) REFERENCES mattrics.Pressure(_id),
    FOREIGN KEY (movementId) REFERENCES mattrics.Movement(_id),
    FOREIGN KEY (breathingId) REFERENCES mattrics.Breathing(_id)
);
