<?php

namespace App\Http\Livewire;

use Livewire\Component;

class FormContact extends Component
{

    public $name;
    public $number;
    public $sucessMessage;


    protected $rules = [
        'name' => 'required|min:6',
        'number' => 'required|numeric|min:3',
    ];

    public function updated($propertyName)
    {
        $this->validateOnly($propertyName);
    }


    public function create(){
        
        //validando
        $validatedData = $this->validate();
        //salvando na db

        //resetando as váriaveis
        $this->reset(['name','number']);
       
        //setando a mensagem de sucesso
        $this->sucessMessage = ' Contato cadastrado! ';
    }

    public function resetMessage() {
        $this->sucessMessage = null;
    }
}
