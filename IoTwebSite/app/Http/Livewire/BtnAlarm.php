<?php

namespace App\Http\Livewire;

use Livewire\Component;

class BtnAlarm extends Component
{
    public $state;
    public function render()
    {
        return view('livewire.btn-alarm');
    }

    public function setState(){
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
