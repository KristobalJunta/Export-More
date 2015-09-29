#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "bcm2835.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

#ifdef RPI
    bcm2835_init();

    // Set the pins to be an output
    bcm2835_gpio_fsel(PIN1, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(PIN2, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(PIN3, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(PIN4, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(PIN5, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(PIN6, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(PIN7, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(PIN8, BCM2835_GPIO_FSEL_OUTP);

    bcm2835_i2c_begin();
    bcm2835_i2c_setSlaveAddress(0x29);      // addr pin attached to ground
    bcm2835_i2c_set_baudrate(1000);         // Default

    bcm2835_spi_begin();
    bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);
    bcm2835_spi_setDataMode(BCM2835_SPI_MODE3);				//SCLK rising edge - clock idle state 1
    bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_65536); 	//set clock frequency
    bcm2835_spi_chipSelect(BCM2835_SPI_CS1);                      	//use chip select 1
    bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS1, LOW);      	//chip select 0 to activate

#endif


    timer.setInterval(500);
    connect(&timer,SIGNAL(timeout()),this,SLOT(connectTCP()));
    connect(&socket,SIGNAL(readyRead()),this,SLOT(readTCP()));
    connect(&socket,SIGNAL(connected()),this,SLOT(connectedTCP()));
    connect(ui->Tempsensor,SIGNAL(valueChanged(int)),this,SLOT(tempChanged(int)));
    connect(ui->LightSensor,SIGNAL(valueChanged(int)),this,SLOT(lightChanged(int)));

#ifdef RPI
    sensors.setInterval(500);
    connect(&sensors,SIGNAL(timeout()),this,SLOT(readSensor()));
#endif

    timer.start();              //start trying get tcp connection

}

void MainWindow::connectTCP()
{
    socket.connectToHost("localhost",42001);
}

void MainWindow::tempChanged(int data)
{
    QByteArray data_send;
    qDebug() << "send sensor update";

        data_send.append((char)0x00);
        data_send.append((char)0x00);
        data_send.append((char)0x00);
        data_send.append((char)0x18);
        data_send.append("sensor-update \"temp\" ");
        data_send.append((char)((data/10)+0x30));
        data_send.append((char)((data%10)+0x30));
        data_send.append(" ");
        socket.write(data_send);


}

void MainWindow::lightChanged(int data)
{

    QByteArray data_send;

        data_send.append((char)0x00);
        data_send.append((char)0x00);
        data_send.append((char)0x00);
        data_send.append((char)0x1c);
        data_send.append("sensor-update \"light\" ");
        data_send.append((char)((data/10000)+0x30));
        data_send.append((char)((data%10000/1000)+0x30));
        data_send.append((char)((data%1000/100)+0x30));
        data_send.append((char)((data%100/10)+0x30));
        data_send.append((char)((data%10)+0x30));
        data_send.append(" ");
        socket.write(data_send);

}

void MainWindow::readTCP()
{
    QString data;
    QByteArray data_byte;
    QByteArray data1;
    int begin,aantal;
    data_byte = socket.readAll();
    begin = data_byte.indexOf("\"") + 1;
    aantal = data_byte.count() - begin - 1;
    data1 = data_byte.mid(begin,aantal);
    qDebug() << data_byte;
    qDebug() << "index of: " << begin << ":" << aantal;
    qDebug() << "data inhoud: " << data_byte.mid(begin,aantal);
    ui->data_label->setText(data_byte.mid(begin,aantal));

    if(data1.contains("led1on") == true )
    {
        ui->led1->setChecked(true);
        #ifdef RPI
        bcm2835_gpio_write(PIN1, HIGH);
        #endif
    }
    if(data1.contains("led1off") == true )
    {
        ui->led1->setChecked(false);
        #ifdef RPI
        bcm2835_gpio_write(PIN1, LOW);
        #endif
    }

    if(data1.contains("led2on") == true )
    {
        ui->led2->setChecked(true);
        #ifdef RPI
        bcm2835_gpio_write(PIN2, HIGH);
        #endif
    }
    if(data1.contains("led2off") == true )
    {
        ui->led2->setChecked(false);
        #ifdef RPI
        bcm2835_gpio_write(PIN2, LOW);
        #endif
    }

    if(data1.contains("led3on") == true )
    {
        ui->led3->setChecked(true);
        #ifdef RPI
        bcm2835_gpio_write(PIN3, HIGH);
        #endif
    }
    if(data1.contains("led3off") == true )
    {
        ui->led3->setChecked(false);
        #ifdef RPI
        bcm2835_gpio_write(PIN3, LOW);
        #endif
    }

    if(data1.contains("led4on") == true )
    {
        ui->led4->setChecked(true);
        #ifdef RPI
        bcm2835_gpio_write(PIN4, HIGH);
        #endif
    }
    if(data1.contains("led4off") == true )
    {
        ui->led4->setChecked(false);
        #ifdef RPI
        bcm2835_gpio_write(PIN4, LOW);
        #endif
    }

    if(data1.contains("led5on") == true )
    {
        ui->led5->setChecked(true);
        #ifdef RPI
        bcm2835_gpio_write(PIN5, HIGH);
        #endif
    }
    if(data1.contains("led5off") == true )
    {
        ui->led5->setChecked(false);
        #ifdef RPI
        bcm2835_gpio_write(PIN5, LOW);
        #endif
    }

    if(data1.contains("led6on") == true )
    {
        ui->led6->setChecked(true);
        #ifdef RPI
        bcm2835_gpio_write(PIN6, HIGH);
        #endif
    }
    if(data1.contains("led6off") == true )
    {
        ui->led6->setChecked(false);
        #ifdef RPI
        bcm2835_gpio_write(PIN6, LOW);
        #endif
    }

    if(data1.contains("led7on") == true )
    {
        ui->led7->setChecked(true);
        #ifdef RPI
        bcm2835_gpio_write(PIN7, HIGH);
        #endif
    }
    if(data1.contains("led7off") == true )
    {
        ui->led7->setChecked(false);
        #ifdef RPI
        bcm2835_gpio_write(PIN7, LOW);
        #endif
    }

    if(data1.contains("led8on") == true )
    {
        ui->led8->setChecked(true);
        #ifdef RPI
        bcm2835_gpio_write(PIN8, HIGH);
        #endif
    }
    if(data1.contains("led8off") == true )
    {
        ui->led8->setChecked(false);
        #ifdef RPI
        bcm2835_gpio_write(PIN8, LOW);
        #endif
    }

}

void MainWindow::connectedTCP()
{
    QByteArray data;
    qDebug() << "TCP connected";

    timer.stop();                           //stop the get connected timer
    emit ui->LightSensor->valueChanged(0);  //send 0 to scratch
    emit ui->Tempsensor->valueChanged(0);   //send 0 to scratch
#ifdef RPI
    sensors.start();                        //start reading sensors
#endif
}

MainWindow::~MainWindow()
{
    delete ui;
}

#ifdef RPI
void MainWindow::readSensor()
{
        //read temperature

        char buffer[4];
        int temp;
        char tmp[2];
        char ad[2];
        int light;
        buffer[0]=buffer[1]=buffer[2]=buffer[3]=0;

        buffer[0] = 0x58;						//read the id
        bcm2835_spi_transfern(buffer,2);
        qDebug() << buffer[1];

        buffer[0] = 0x50;					//read the temp
        bcm2835_spi_transfern(buffer,3);
        qDebug() << "status: " << buffer[1] << buffer[2];

        temp = buffer[1];
        temp = temp<<8;
        temp = temp + ( buffer[2] & 0xF8);
        temp = temp>>3;
        temp = temp/16;
        ui->Tempsensor->setValue(temp);

        //read light

        tmp[0] = 0xa0;				//select the control register
        bcm2835_i2c_write(tmp,1);
        tmp[0] = 0x03;				//Power up the device
        bcm2835_i2c_write(tmp,1);
        bcm2835_delay(500);

        bcm2835_i2c_read(tmp,1);
        qDebug() << "if 33 the device is turned on: " << tmp[0];

        tmp[0] = 0xac;				//Channel 0 lower byte
        bcm2835_i2c_write(tmp,1);
        bcm2835_i2c_read(tmp,1);

        ad[1]= (int)tmp[0];

        tmp[0] = 0xad;				//channel 0 upper byte
        bcm2835_i2c_write(tmp,1);
        bcm2835_i2c_read(tmp,1);

        ad[0] = (int)tmp[0];
        light = ad[0]*256+ad[1];
        ui->LightSensor->setValue(light);

}
#endif