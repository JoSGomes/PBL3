<?php

namespace App\Http\Livewire;

use Livewire\Component;

class FormContact extends Component
{

    public $name;
    public $number;
    public $sucessMessage;

    public function create(){
        
        //salvando na db

        //resetando as váriaveis
        $this->reset(['name','number']);
       
        //setando a mensagem de sucesso
        $this->sucessMessage = ' Contato salvo com sucesso ';
    }

    public function resetMessage() {
        $this->sucessMessage = null;
    }
}
