#include "database.hpp"

static char qTBuffer[256] = {0};
/********************************************************************
Fuction: QueryVistorsNum
Description: Query Visitor information
Parameter: struct personAttribute *perAttrPt, string str, struct qryCond *qryPt
Return Value:
Author:LiuYonggang
date:2019.10.21
********************************************************************/
int QueryVistorsNum(struct personAttribute *perAttrPt, string str, struct qryCond *qryPt)
{
    int cnt = 0;
    QSqlQuery qry(db);

    snprintf(qTBuffer, sizeof(qTBuffer), "SELECT * FROM %s ", str.c_str());

    if( !qry.exec(qTBuffer) )   // Query Table Failure
    {
        qDebug("Excute Query Person number in time internal fail");
        return -1;
    }
    else
    {
#if DEBUG_MODE
	qDebug("Excute Query Person number in time internal Success!");
        qDebug ("Query logNum number:%d", visitorNum);
#endif
    }


    snprintf(qTBuffer, sizeof(qTBuffer), "SELECT * FROM %s ", str.c_str());
    qry.exec(qTBuffer);

    QSqlRecord rec = qry.record();

    for( cnt = 0; (cnt < qryPt->startAddr) && qry.next(); cnt++ );

    if (cnt != qryPt->startAddr)
    {
        qDebug("Query Visitor log Postion error");
    }

    qry.previous();

    for( cnt = 0; (cnt < qryPt->dataLenth) && qry.next(); cnt++ )
    {

        perAttrPt[cnt].id = qry.value(0).toInt();
        QString name = qry.value(1).toString();
        perAttrPt[cnt].name = name.toStdString();
        QString sex = qry.value(2).toString();
        perAttrPt[cnt].sex = sex.toStdString();
        perAttrPt[cnt].age = qry.value(3).toInt();

        QString expression = qry.value(4).toString();
        perAttrPt[cnt].expression = expression.toStdString();
        QString clothcolor = qry.value(5).toString();
        perAttrPt[cnt].clothcolor = clothcolor.toStdString();
        QString hair = qry.value(6).toString();
        perAttrPt[cnt].hair = hair.toStdString();
        QString glass = qry.value(7).toString();
        perAttrPt[cnt].glass = glass.toStdString();
        QString sleeves = qry.value(8).toString();
        perAttrPt[cnt].sleeves = sleeves.toStdString();
        QString time = qry.value(9).toString();
        perAttrPt[cnt].time = time.toStdString();

    }

    if (cnt != qryPt->dataLenth)
    {
        qDebug("Query Vistors log Number error");
    }

    return cnt;
}

/********************************************************************
Fuction: AddOnePersonAttribute
Description: Add a member attribute into PersonAttr Table
Parameter: struct personAttribute *pt, string str
Return Value: enum RETVALUE
Author: LiuYonggang
date: 2019.10.21
********************************************************************/
enum RETVALUE AddOnePersonAttribute(struct personAttribute *pt, string str)
{
    QSqlQuery qry(db);

    snprintf(qTBuffer, sizeof(qTBuffer), "SELECT * FROM %s WHERE name='%s'", str.c_str() , pt->name.c_str());

    if( !qry.exec(qTBuffer) )   // Query Table Failure
    {
        qDebug("Excute Query table fail");
        return FAIL;
    }
    else
    {
#if DEBUG_MODE
       qDebug( "Excute Query table Successful!" );
#endif
       //QSqlRecord rec = qry.record();

       QSqlQueryModel *model = new QSqlQueryModel();
       model->setQuery(qry);
       int nRecordCount = model->rowCount();
       delete model;

       if (nRecordCount >= 1)
       {
#if DEBUG_MODE
	   qDebug() << ("=============nRecordCount:%d", nRecordCount);
           qDebug( "The Member %d has already existed and exit directly", pt->id);
#endif
	   return EXIST;
       }
    }
    snprintf(qTBuffer, sizeof(qTBuffer), "INSERT INTO %s VALUES(%d, '%s', '%s', %d, '%s', '%s','%s','%s', '%s', '%s')", str.c_str(), pt->id, pt->name.c_str(), pt->sex.c_str(), pt->age,  pt->expression.c_str(), pt->clothcolor.c_str(), pt->hair.c_str(), pt->glass.c_str(), pt->sleeves.c_str(), pt->time.c_str() );

    if( !qry.exec(qTBuffer) )
    {
        qDebug() << "Insert Member failure!";
        return FAIL;
    }
    else
    {
#if DEBUG_MODE
        qDebug( "Insert Member Succesful!" );
#endif
        return SUCC;
    }
}

/********************************************************************
Fuction: DeleteOnePersonAttribute
Description: Delete a member attribute from PersonAttr Table
Parameter: struct personAttribute *pt, string str
Return Value: enum RETVALUE
Author: LiuYonggang
date: 2019.10.21
********************************************************************/
enum RETVALUE DeleteOnePersonAttribute(struct personAttribute *pt, string str)
{
    QSqlQuery qry(db);

    snprintf(qTBuffer, sizeof(qTBuffer), "DELETE FROM %s WHERE name = '%s'", str.c_str(), pt->name.c_str());

    if( !qry.exec(qTBuffer) )
    {
        qDebug() << "Execute Delete Command Failure";
        return FAIL;
    }
    else
    {
#if DEBUG_MODE
	qDebug() << "Execute Delete Command Sucessfully";
#endif
	return SUCC;
    }
}

/********************************************************************
Fuction: UpdateOnePersonAttribute
Description: Add a member attribute into PersonAttr Table
Parameter: struct personAttribute *pt
Return Value:
Author: LiuYonggang
date: 2019.10.21
********************************************************************/
bool UpdateOnePersonAttribute(int idVale)
{
    QSqlQuery qry(db);

    snprintf(qTBuffer, sizeof(qTBuffer), "UPDATE PersonAttr SET hair = 'long' WHERE id = %d",idVale);

    if( !qry.exec(qTBuffer) )
    {
        qDebug() << "Update Member Failure!";
        return false;
    }
    else
    {
#if DEBUG_MODE
        qDebug( "Update Member Suceesful!" );
#endif
	return true;
    }
}

/********************************************************************
Fuction: QueryTableAccordingTime
Description: Query log at some gap and return log information
Parameter: struct personAttribute *perAttrPt, string str, struct qryCond *qryPt
Return Value: int, struct personAttribute *perAttrPt
Author: LiuYonggang
date: 2019.10.21
********************************************************************/
int QueryTableAccordingTime(struct personAttribute *perAttrPt, string str, struct qryCond *qryPt)
{
    int cnt = 0;
    QSqlQuery qry(db);

    if (0 == qryPt->flag)
    {
        snprintf(qTBuffer, sizeof(qTBuffer), "SELECT * FROM %s WHERE time > '%s' and time < '%s' ", str.c_str(), qryPt->startTime.c_str(), qryPt->endTime.c_str());

        if( !qry.exec(qTBuffer) )   // Query Table Failure
        {
            qDebug("Excute Query Person number in time internal fail");
            return -1;
        }
        else
        {
    	    QSqlQueryModel *model = new QSqlQueryModel();
            model->setQuery(qry);
            int logNum = model->rowCount();
	    delete model;
#if DEBUG_MODE
            qDebug ("Query logNum number:%d", logNum);
#endif
            return logNum;
        }
    }
    else if (1 == qryPt->flag)
    {
        snprintf(qTBuffer, sizeof(qTBuffer), "SELECT * FROM %s WHERE time > '%s' and time < '%s' ", str.c_str(), qryPt->startTime.c_str(), qryPt->endTime.c_str());
        qry.exec(qTBuffer);

        QSqlRecord rec = qry.record();

        for( cnt = 0; (cnt < qryPt->startAddr) && qry.next(); cnt++ );

        if (cnt != qryPt->startAddr)
        {
            qDebug("Query log Postion error");
        }

        qry.previous();
        for( cnt = 0; (cnt < qryPt->dataLenth) && qry.next(); cnt++ )
        {
            perAttrPt[cnt].id = qry.value(0).toInt();
            QString name = qry.value(1).toString();
            perAttrPt[cnt].name = name.toStdString();
            QString sex = qry.value(2).toString();
            perAttrPt[cnt].sex = sex.toStdString();
            perAttrPt[cnt].age = qry.value(3).toInt();

            QString expression = qry.value(4).toString();
            perAttrPt[cnt].expression = expression.toStdString();
            QString clothcolor = qry.value(5).toString();
            perAttrPt[cnt].clothcolor = clothcolor.toStdString();
            QString hair = qry.value(6).toString();
            perAttrPt[cnt].hair = hair.toStdString();
            QString glass = qry.value(7).toString();
            perAttrPt[cnt].glass = glass.toStdString();
            QString sleeves = qry.value(8).toString();
            perAttrPt[cnt].sleeves = sleeves.toStdString();
            QString time = qry.value(9).toString();
            perAttrPt[cnt].time = time.toStdString();
	}

        if (cnt != qryPt->dataLenth)
        {
            qDebug("Query log Number error");
        }
    }
	
    return cnt;
}

/********************************************************************
Fuction: QueryTotalPerson
Description: Query male,female,total number
Parameter: string str, string type
Return Value:
Author:LiuYonggang
date:2019.10.21
********************************************************************/
int QueryTotalPerson(string str, string type)
{
    QSqlQueryModel *model = new QSqlQueryModel();
    int maleCnt,femaleCnt;
    QSqlQuery qry(db);

    // Query male number
    snprintf(qTBuffer, sizeof(qTBuffer), "SELECT * FROM %s WHERE sex='男'", str.c_str());
    if( !qry.exec(qTBuffer) )   // Query Table Failure
    {
        qDebug("Excute Query table fail");
	delete model;
        return -1;
    }
    else
    {
        //QSqlRecord rec = qry.record();
        model->setQuery(qry);
        maleCnt = model->rowCount();
#if DEBUG_MODE
	qDebug( "Excute Query table Successful!" );
        qDebug ("Male number:%d", maleCnt);
#endif
    }

    // Query female number
    snprintf(qTBuffer, sizeof(qTBuffer), "SELECT * FROM %s WHERE sex='女'", str.c_str());
    if( !qry.exec(qTBuffer) )   // Query Table Failure
    {
        qDebug("Excute Query table fail");
	delete model;
        return -1;
    }
    else
    {
        //QSqlRecord rec = qry.record();
        model->setQuery(qry);
        femaleCnt = model->rowCount();
#if DEBUG_MODE
	qDebug( "Excute Query table Successful!" );
        qDebug ("Female Number:%d", femaleCnt);
#endif
    }

    delete model;

    if (0 == strcmp(type.c_str(), "male"))
    {
        return maleCnt;
    }
    else if (0 == strcmp(type.c_str(), "female"))
    {
        return femaleCnt;
    }
    else if (0 == strcmp(type.c_str(), "all"))
    {
        return maleCnt + femaleCnt;
    }
    return 0;
}

/********************************************************************
Fuction: QueryTableAccordingEm
Description: Query "neutral", "happy", "sad", "surprise", "anger
Parameter: string str, string type
Return Value:
Author:lijian
date:2019.10.30
********************************************************************/
int QueryTableAccordingEm(string str, int *em)
{

    if(NULL == em)
        return -1;

    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQuery qry(db);

    auto emotionsVecSize = emotionsVec.size();

    for (size_t i = 0; i < emotionsVecSize; i++) {
        snprintf(qTBuffer, sizeof(qTBuffer), "SELECT * FROM %s WHERE expression=\"%s\"", str.c_str() , emotionsVec[i].c_str());
        if(false == qry.exec(qTBuffer) )   // Query Table Failure
        {
            qDebug("Excute Query table fail");
            delete model;
            return -1;
        }
        else
        {
            model->setQuery(qry);
            em[i] = model->rowCount();
    #if DEBUG_MODE
        qDebug( "Excute Query table Successful!" );
            qDebug ("Male number:%d", maleCnt);
    #endif
        }
    }

    return 0;
}

/********************************************************************
Fuction: QueryWholeTable
Description: Query all members from PersonAttr Table
Parameter: string str
Return Value: enum RETVALUE
Author: LiuYonggang
date: 2019.10.21
********************************************************************/
enum RETVALUE QueryWholeTable(string str)
{
    QSqlQuery qry(db);

    snprintf(qTBuffer, sizeof(qTBuffer), "SELECT * FROM %s", str.c_str());

    if( !qry.exec(qTBuffer) )
    {
        qDebug() << "Query The Whole Table Failure!";
        return FAIL;
    }
    else
    {
#if DEBUG_MODE
        qDebug( "Selected Updated Table Successful!" );
#endif
	QSqlRecord rec = qry.record();

        for( int r=0; qry.next(); r++ )
        {
            int id = qry.value(0).toInt();
            QString name = qry.value(1).toString();
            QString sex = qry.value(2).toString();
            int age = qry.value(3).toInt();

            QString expression = qry.value(4).toString();
            QString clothcolor = qry.value(5).toString();
            QString hair = qry.value(6).toString();
            QString glass = qry.value(7).toString();
            QString sleeves = qry.value(8).toString();
            QString time = qry.value(9).toString();

            qDebug()<<QString("ID:%1 Name:%2 Sex:%3 Age:%4 Expre:%5 Col:%6 Hair:%7 Gls:%8 Sle:%9 Tim:%10").arg(id).arg(name).arg(sex).arg(age).arg(expression).arg(clothcolor).arg(hair).arg(glass).arg(sleeves).arg(time);
        }

        return SUCC;
    }

}

/********************************************************************
Fuction: CreatePersonAttributeTab
Description: Create Person Attributes Table
Parameter: string str
Return Value: enum RETVALUE
Author: LiuYonggang
date: 2019.10.21
********************************************************************/
enum RETVALUE CreatePersonAttributeTab(string str)
{
    QSqlQuery qry(db);

    snprintf(qTBuffer, sizeof(qTBuffer), "CREATE TABLE IF NOT EXISTS %s(id INTEGER UNIQUE PRIMARY KEY, name varchar(20), sex varchar(10), age int, expression varchar(10), clothcolor varchar(10), hair varchar(10), glass varchar(10), sleeves varchar(10), time varchar(30))", str.c_str());
    if(!qry.exec(qTBuffer))
    {
        qDebug() << "PersonAttr Table created Failure!";
        return FAIL;
    }
    else
    {
#if DEBUG_MODE
        qDebug() << "PersonAttr Table created Sucessful!";
#endif
	return SUCC;
    }
}

/********************************************************************
Fuction: CreateDatabaseConnectionSwitch
Description: Create database
Parameter: QSqlDatabase db, bool on_off
Return Value: enum RETVALUE
Author: LiuYonggang
date: 2019.10.21
********************************************************************/
enum RETVALUE CreateDatabaseConnectionSwitch(QSqlDatabase db, bool on_off)
{

    if (ON == on_off)
    {
        db = QSqlDatabase::addDatabase( "QSQLITE" );            // add database
        db.setDatabaseName( "localhost" );                      // set database name

        if(!db.open())  // open connection
        {
            std::cout <<  "Connection Database Failure!" << std::endl;         // qFatal
            return FAIL;
        }
        else
        {
            std::cout << "Connected Database Successful!"<< std::endl;
        }
    }
    else
    {
        db.close();
    }

    return SUCC;
}




/********************************************************************
Fuction: CreateMeetingTab
Description: Create Meeting Table
Parameter: string str
Return Value: enum RETVALUE
Author: lijian
date: 2019.11.05
********************************************************************/
enum RETVALUE CreateMeetingTab(string str)
{
    QSqlQuery qry(db);

    snprintf(qTBuffer, sizeof(qTBuffer), "CREATE TABLE IF NOT EXISTS %s(name varchar(20), start_time varchar(30) , end_time varchar(30), place varchar(30) )", str.c_str());
    if(!qry.exec(qTBuffer))
    {
        qDebug() << "Meeting Table created Failure!";
        return FAIL;
    }
    else
    {
#if DEBUG_MODE
        qDebug() << "PersonAttr Table created Sucessful!";
#endif
    return SUCC;
    }
}


/********************************************************************
Fuction: AddOnePerson2Meeting
Description: Add a people to Meeting Table
Parameter: struct personAttribute *pt, string str
Return Value: enum RETVALUE
Author: lijian
date: 2019.11.05
********************************************************************/
enum RETVALUE AddOnePerson2Meeting(MeetingInfoStr *pt, string tab)
{
    QSqlQuery qry(db);

    snprintf(qTBuffer, sizeof(qTBuffer), "SELECT * FROM %s WHERE name='%s' and start_time='%s' and end_time='%s'", tab.c_str() , pt->name.c_str() ,pt->start_time.c_str(), pt->end_time.c_str());
    std::cout << "cmd:" << qTBuffer << std::endl;
    if(false == qry.exec(qTBuffer) )   // Query Table Failure
    {
        qDebug("Excute Query table fail");
        return FAIL;
    }
    else
    {
       QSqlQueryModel *model = new QSqlQueryModel();
       model->setQuery(qry);
       int nRecordCount = model->rowCount();
       delete model;

       if (nRecordCount >= 1)
       {
            return EXIST;
       }
    }
    snprintf(qTBuffer, sizeof(qTBuffer), "INSERT INTO %s VALUES('%s', '%s', '%s', '%s')", tab.c_str(), pt->name.c_str(), pt->start_time.c_str(), pt->end_time.c_str(),  pt->place.c_str());

    if( !qry.exec(qTBuffer) )
    {
        qDebug() << "Insert Member failure!";
        return FAIL;
    }
    else
    {
        return SUCC;
    }
}

/********************************************************************
Fuction: PrintWholeMeetingTable
Description: Query all members from Meeting Table
Parameter: string str
Return Value: enum RETVALUE
Author: lijian
date: 2019.11.05
********************************************************************/
enum RETVALUE PrintWholeMeetingTable(string str)
{
    QSqlQuery qry(db);

    snprintf(qTBuffer, sizeof(qTBuffer), "SELECT * FROM %s", str.c_str());

    if( !qry.exec(qTBuffer) )
    {
        qDebug() << "Query The Whole Table Failure!";
        return FAIL;
    }
    else
    {

        //QSqlRecord rec = qry.record();

        for( int r=0; qry.next(); r++ )
        {
            std::cout << qry.value(0).toString().toStdString() << "---";
            std::cout << qry.value(1).toString().toStdString() << "---";
            std::cout << qry.value(2).toString().toStdString() << "---";
            std::cout << qry.value(3).toString().toStdString() << std::endl;
        }
        return SUCC;
    }

}

int PrintGroupByMeetingTable(std::list <MeetingListStr> &meetinglist, string str)
{
    int total = 0;
    MeetingListStr m_list;

    QSqlQuery qry(db);

    snprintf(qTBuffer, sizeof(qTBuffer), "SELECT * FROM %s GROUP BY start_time", str.c_str());

    if( !qry.exec(qTBuffer) )
    {
        qDebug() << "Query The Whole Table Failure!";

    }
    else
    {
        for( int r=0; qry.next(); r++ )
        {
            m_list.start_time =qry.value(1).toString().toStdString();
            m_list.end_time =qry.value(2).toString().toStdString();
            meetinglist.push_back(m_list);

            total++;
        }
    }
    return total;
}

bool GetMeetingListByTime(std::list <MeetingInfoStr> &meetinglistr , string s_tim , string e_tim, string str)
{
    MeetingInfoStr m_list;

    QSqlQuery qry(db);

    snprintf(qTBuffer, sizeof(qTBuffer), "SELECT * FROM %s WHERE start_time='%s' and end_time='%s'", str.c_str() , s_tim.c_str() , e_tim.c_str());

    if( !qry.exec(qTBuffer) )
    {
        qDebug() << "Query The Whole Table Failure!";
        return false;
    }
    else
    {
        for( int r=0; qry.next(); r++ )
        {
            m_list.name =qry.value(0).toString().toStdString();
            m_list.start_time =qry.value(1).toString().toStdString();
            m_list.end_time =qry.value(2).toString().toStdString();
            m_list.place =qry.value(3).toString().toStdString();
            m_list.status = 0;

            meetinglistr.push_back(m_list);
        }
    }
    return true;
}

