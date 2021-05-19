@extends('layouts.template')

@section('body')
    <div class="container">
        <div class="row">
            <div class="col">
                <div id="stateDevice" class="subtitle ">Dispositivo [ESTADO]</div>
                <livewire:btn-alarm/>
            </div>

            <div class="col">
                <h1 class="subtitle">Contatos de emergência</h1>
                <div class="scrollable position-relative top-50">
                    <table class="table table-hover table-bordeless">
                        <thead>
                        <tr>
                            <th scope="col">Nome</th>
                            <th scope="col">Número</th>
                        </tr>
                        </thead>
                        <tbody>
                        @foreach ($contacts as $contact)
                            <tr >
                                 <td>{{$contact->name}}</td>
                                 <td>{{$contact->number}}</td>
                            </tr>
                        @endforeach
                        </tbody>
                    </table>
                </div>

            </div>
        </div>
    </div>
    <script src="https://code.jquery.com/jquery-3.6.0.js" integrity="sha256-H+K7U5CnXl1h5ywQfKtSj8PCmoN9aaq30gDh27Xc0jk=" crossorigin="anonymous"></script>
    <script> 
        $(document).ready(setInterval(function(){
            
        },1000));
    </script>
@endsection


