<?php

namespace App\Http\Livewire;

use Livewire\Component;
use App\Models\Connection;
use App\Models\Configuration;

class StateDevice extends Component
{
    public $verificationTime = 30 * 1000;
    public $state = 'desconectado';


    public function render()
    {
        return view('livewire.state-device');
    }

    public function mount(){
       $connect = Connection::first();
       
        if ($connect->interval <= 120) {
            $this->verificationTime = ($connect->interval/2) *1000;
        } else {
            $this->verificationTime = 30 * 1000;
        }
        
       $hour = explode ( ':' , $connect->hour , 3);
       $hour =  $hour[0] * 3600 +  $hour[1] * 60 +  $hour[2] * 1;
       
       date_default_timezone_set('America/Bahia');
       $actualHour = explode ( ':' , date ( "G:i:s") , 3) ;
       $actualHour =  $actualHour[0] * 3600 +  $actualHour[1] * 60 +  $actualHour[2] * 1; 
       

        if ($hour+$connect->interval >= $actualHour ) {
            $this->state = 'Conectado';
            $connect->value = 'Conectado';
            $connect->save();
        } else {
            $this->state = 'Desconectado';
            $connect->value = 'Desconectado';
            $connect->save();
        }
        
    }

    public function hourVerification(){

    }
}
