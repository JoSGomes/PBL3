<?php

namespace App\Http\Livewire;

use Livewire\Component;
use PhpMqtt\Client\Facades\MQTT;
use App\Models\StateAlarm;

class BtnAlarm extends Component
{
    public $state;
    public function render()
    {
        return view('livewire.btn-alarm');
    }
    public function mount(){
        if(StateAlarm::all()->last()){
            $this->state = StateAlarm::all()->last()->state;
        }    
    }
    public function setState(){
        if(StateAlarm::all()->count() != 0){
            $saveState = StateAlarm::all()->last();
            $saveState->state = $this->state;
            $saveState->save();
        }
        else{
            $saveState = new StateAlarm();
            $saveState->state = $this->state;
            $saveState->save();
        }
        

        if ($this->state) {
            //alarm atv
            $mqtt = MQTT::connection();
            $mqtt->publish('SET_ALARM',1, 0);
            $mqtt->disconnect(); 
            
           

        }else {
            //alarm dest
            $mqtt = MQTT::connection();
            $mqtt->publish('SET_ALARM',0, 0);
            $mqtt->disconnect(); 
        }
        
    }
}
