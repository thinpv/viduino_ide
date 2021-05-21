#include "Wire.h"
#include <i2c.h>
#include <gpio.h>

static int maix_i2c_slave_irq(void *userdata);

TwoWire::TwoWire(I2CPinDescription* i2c) : i2c(i2c)
{

}

// TwoWire::~TwoWire()
// {
//     //clear
// }

void 
TwoWire::begin(uint32_t frequency)
{
    i2c_clk = frequency;
    // uint8_t speed_mode = I2C_CON_SPEED_STANDARD;
    // if((i2c_clk>100000) && (i2c_clk<=1000000))
    //     speed_mode = I2C_CON_SPEED_FAST;
    // else
    //     speed_mode = I2C_CON_SPEED_HIGH;
    is_master_mode = true;
    isBusy = false;
    
    i2c_tx_buff = RingBuffer();
    i2c_rx_buff = RingBuffer();

	gpio_set_cfg(i2c->pintype[0].P, i2c->pintype[0].num, i2c->pintype[0].type);
    gpio_set_cfg(i2c->pintype[1].P, i2c->pintype[1].num, i2c->pintype[1].type);
	gpio_set_pull(i2c->pintype[0].P, i2c->pintype[0].num, GPIO_PULL_UP);
	gpio_set_pull(i2c->pintype[1].P, i2c->pintype[1].num, GPIO_PULL_UP);

	i2c_set(i2c->I, i2c_clk);	
}
    
// void 
// TwoWire::begin(uint16_t slave_address)
// {
//     is_master_mode = false;
//     i2c_tx_buff = new RingBuffer();
//     i2c_rx_buff = new RingBuffer();
// }  

void
TwoWire::setTimeOut(uint16_t timeOutMillis)
{
    _timeOutMillis = timeOutMillis;
}

uint16_t 
TwoWire::getTimeOut()
{
    return _timeOutMillis;
}

void 
TwoWire::setClock(uint32_t frequency)
{
    i2c_clk = frequency;
    // uint8_t speed_mode = I2C_CON_SPEED_STANDARD;
    // if((i2c_clk>100000) && (i2c_clk<=1000000))
    //     speed_mode = I2C_CON_SPEED_FAST;
    // else
    //     speed_mode = I2C_CON_SPEED_HIGH;
	i2c_set_rate(i2c->I, i2c_clk);
}

uint32_t 
TwoWire::getClock()
{
    return i2c_clk;
}

int
TwoWire::writeTransmission(uint16_t address, uint8_t* send_buf, size_t send_buf_len, bool sendStop)
{
    if(!isBusy){
        if(i2c_start(i2c->I))
            isBusy = true;
        else 
            return 0;
    }
	int value = i2c_write(i2c->I, address, send_buf, send_buf_len);
    if(sendStop){
        if(i2c_stop(i2c->I))
            isBusy = false;
    }
    return value;
}

int
TwoWire::readTransmission(uint16_t address, uint8_t* receive_buf, size_t receive_buf_len, bool sendStop)
{
    if(!isBusy){
        if(i2c_start(i2c->I))
            isBusy = true;
        else 
            return 0;
    }
	int value = i2c_read(i2c->I, address, receive_buf, receive_buf_len);
    if(sendStop){
        if(i2c_stop(i2c->I))
            isBusy = false;
    }
    return value;
}

void 
TwoWire::beginTransmission(uint16_t address)
{
    transmitting = 1;
    txAddress = address;
}

uint8_t 
TwoWire::endTransmission(bool sendStop)  //结束时从rxbuff发送数据？
{
    int state = -1;
    int index = 0;
    uint8_t temp = 0;
    size_t tx_data_length = i2c_tx_buff.available();
    if(tx_data_length == 0){
        state = readTransmission(txAddress, &temp, 1, sendStop);
        return state;
    }
    uint8_t tx_data[RING_BUFFER_SIZE];
    while(i2c_tx_buff.available())
    {
        tx_data[index++] = i2c_tx_buff.read_char();
    }
    state = writeTransmission(txAddress, tx_data, tx_data_length, sendStop);
    return state;
}

uint8_t
TwoWire::requestFrom(uint16_t address, uint8_t size, bool sendStop)  //请求数据，存入rxbuff，供read读
{
    int state,index = 0;
    uint8_t rx_data[RING_BUFFER_SIZE];
    state = readTransmission(address, rx_data, size, sendStop);
    // if(0 == state){
        while(size)
        {
            i2c_rx_buff.store_char(rx_data[index++]); 
            size--;
        }
    // }
    return i2c_rx_buff.available();
}

size_t 
TwoWire::write(uint8_t data) //写到txbuff
{
    if(transmitting) {
        i2c_tx_buff.store_char(data);
    }
    return 0;
}

size_t 
TwoWire::write(const uint8_t *data, int quantity)
{
    for(size_t i = 0; i < quantity; ++i) {
        if(!write(data[i])) {
            return i;
        }
    }
    return quantity;
}

int TwoWire::available(void)   //rxbuff.available
{
    return i2c_rx_buff.available();
}

int TwoWire::read(void)    //rxbuff.read
{
    return i2c_rx_buff.read_char();
}

int TwoWire::peek(void)    
{ 
    return i2c_rx_buff.peek();
}

void TwoWire::flush(void)
{
    i2c_rx_buff.clear();
    i2c_tx_buff.clear();
}

uint8_t TwoWire::requestFrom(uint8_t address, uint8_t quantity, uint8_t sendStop)
{
    return requestFrom(static_cast<uint16_t>(address), static_cast<size_t>(quantity), static_cast<bool>(sendStop));
}

uint8_t TwoWire::requestFrom(uint16_t address, uint8_t quantity, uint8_t sendStop)
{
    return requestFrom(address, static_cast<size_t>(quantity), static_cast<bool>(sendStop));
}

uint8_t TwoWire::requestFrom(uint8_t address, uint8_t quantity)
{
    return requestFrom(static_cast<uint16_t>(address), static_cast<size_t>(quantity), true);
}

uint8_t TwoWire::requestFrom(uint16_t address, uint8_t quantity)
{
    return requestFrom(address, static_cast<size_t>(quantity), true);
}

uint8_t TwoWire::requestFrom(int address, int quantity)
{
    return requestFrom(static_cast<uint16_t>(address), static_cast<size_t>(quantity), true);
}

uint8_t TwoWire::requestFrom(int address, int quantity, int sendStop)
{
    return static_cast<uint8_t>(requestFrom(static_cast<uint16_t>(address), static_cast<size_t>(quantity), static_cast<bool>(sendStop)));
}

void TwoWire::beginTransmission(int address)
{
    beginTransmission(static_cast<uint16_t>(address));
}

void TwoWire::beginTransmission(uint8_t address)
{
    beginTransmission(static_cast<uint16_t>(address));
}

uint8_t TwoWire::endTransmission(void)
{
    return endTransmission(true);
}

bool 
TwoWire::busy(void){ 
    return isBusy;
}

void 
TwoWire::onReceive( void (*handler)(int) )
{
    slave_recv_handler = handler;
}
void
TwoWire::onRequest( void (*handler)(void) )
{
    slave_send_handler = handler;
}

void
TwoWire::on_receive(uint8_t rec_data)
{
    if(slave_irq_event_start)
    {
        i2c_rx_buff.store_char(rec_data);
        (*slave_recv_handler)((int)1);//此处跳到接收事件函数
    }
    return;
}

uint8_t
TwoWire::on_transmit()
{
    if(slave_irq_event_start){
        (*slave_send_handler)();//跳到发送事件函数
        return i2c_tx_buff.read_char();
    }
    return 0;
}

uint8_t 
TwoWire::on_event(i2c_event_t event)
{
    if(event == I2C_EV_START)
        slave_irq_event_start = true ;
    else
        slave_irq_event_start = false;
    return 0;
}

static int maix_i2c_slave_irq(void *userdata)
{
    // auto &driver = *reinterpret_cast<TwoWire *>(userdata); 
    // volatile i2c_t *i2c_adapter = i2c[driver._i2c_num];
    // uint32_t status = i2c_adapter->intr_stat;
    // if (status & I2C_INTR_STAT_START_DET)
    // {
    //     driver.on_event(I2C_EV_START);
    //     readl(&i2c_adapter->clr_start_det);
    // }
    // if (status & I2C_INTR_STAT_RX_FULL)
    // {
    //     driver.on_receive(i2c_adapter->data_cmd);
    // }
    // if (status & I2C_INTR_STAT_RD_REQ)
    // {
    //     i2c_adapter->data_cmd = driver.on_transmit();
    //     readl(&i2c_adapter->clr_rd_req);
    // }
    // if (status & I2C_INTR_STAT_STOP_DET)
    // {
    //     driver.on_event(I2C_EV_STOP);
    //     readl(&i2c_adapter->clr_stop_det);
    // }
    return 0;
}


void
TwoWire::scan(int start, int end){
    uint8_t temp;
    for (int addr = start; addr < end; ++addr) {
        // int ret = i2c_p->writeto(self, addr, NULL, 0, true);
        // printf("find %x\n",addr);
        int ret = readTransmission(addr,&temp, 1, 1);
        // printf("ret:%x\n",ret);
        if (ret == 1) {
            Serial.print("SCAN Find device: 0x");
            Serial.println(addr,HEX);
        }
    }
    Serial.println("SCAN done");
}

TwoWire Wire(&I2C_Desc[0]);
TwoWire Wire1(&I2C_Desc[2]);
TwoWire Wire2(&I2C_Desc[3]);